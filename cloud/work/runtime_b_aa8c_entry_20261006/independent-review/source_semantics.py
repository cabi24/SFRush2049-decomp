"""Interpret actual AA8C research C as a bounded abstract source contract.

No compiler, emitted function, added brace/suffix, private ABI experiment or C
execution claim. pycparser accepts exactly one incomplete outer compound at EOF;
its open marker remains explicit. Only the C subset present in this packet is
supported; every other AST node/type fails closed. Integer storage is modeled as
8/16/32-bit target types; signed narrowing models the established IDO/MIPS rule.
"""
import re
from dataclasses import dataclass
from pycparser import c_parser, c_ast


def clean(text):
    text = re.sub(r'/\*.*?\*/|//[^\n]*', '', text, flags=re.S)
    allowed = {'#ifndef RUNTIME_B_AA8C_ENTRY_LAYOUTS_H',
               '#define RUNTIME_B_AA8C_ENTRY_LAYOUTS_H', '#endif', '#include "layouts.h"'}
    for line in text.splitlines():
        if line.strip().startswith('#'): assert line.strip() in allowed, ('unsupported directive', line)
    return re.sub(r'^\s*#[^\n]*', '', text, flags=re.M)


class PrefixParser(c_parser.CParser):
    def __init__(self):
        super().__init__()
        self.depth, self.open_prefixes = 0, 0

    def _parse_compound_statement(self):
        token = self._expect('LBRACE')
        self.depth += 1
        items = self._parse_block_item_list()
        if self._peek_type() is None:
            assert self.depth == 1, 'only an outer function-prefix may be incomplete'
            self.open_prefixes += 1
        else:
            self._expect('RBRACE')
        self.depth -= 1
        return c_ast.Compound(items, coord=self._tok_coord(token))


@dataclass
class Type:
    kind: str
    size: int = 0
    align: int = 1
    signed: bool = False
    element: object = None
    fields: object = None


class Returned(Exception):
    pass


class Source:
    GLOBALS = {'D_80399120': 0x80399120, 'D_80399550': 0x80399550,
               'D_80399118': 0x80399118, 'D_80152818': 0x80152818,
               'D_8014A250': 0x8014A250, 'D_80394884': 0x80394884}

    def __init__(self, header, helpers, prefix):
        parser = PrefixParser()
        tree = parser.parse(clean(header + '\n' + helpers + '\n' + prefix))
        assert parser.open_prefixes == 1
        self.types, self.globals, self.functions = {}, {}, {}
        for n in tree.ext:
            if isinstance(n, c_ast.Typedef): self.types[n.name] = self.ctype(n.type)
            elif isinstance(n, c_ast.FuncDef): self.functions[n.decl.name] = n
            elif isinstance(n, c_ast.Decl):
                if not isinstance(n.type, c_ast.FuncDecl):
                    self.globals[n.name] = self.ctype(n.type)
            else: raise AssertionError(('unsupported external node', type(n).__name__))
        assert set(self.globals) == set(self.GLOBALS)
        assert set(self.functions) == {'func_8038A95C', 'func_8038AA14', 'func_8038AA8C'}

    def ctype(self, n):
        if isinstance(n, c_ast.TypeDecl): return self.ctype(n.type)
        if isinstance(n, c_ast.IdentifierType):
            name = ' '.join(n.names)
            if name in self.types: return self.types[name]
            if name in ('signed char', 'unsigned char'): size = 1
            elif name in ('signed short', 'unsigned short'): size = 2
            elif name in ('signed int', 'unsigned int', 'float'): size = 4
            elif name == 'void': return Type('void')
            else: raise AssertionError(('unsupported type', name))
            return Type('scalar', size, size, name.startswith('signed'))
        if isinstance(n, c_ast.PtrDecl): return Type('pointer', 4, 4, element=self.ctype(n.type))
        if isinstance(n, c_ast.ArrayDecl):
            element = self.ctype(n.type)
            count = self.constant(n.dim) if n.dim is not None else 0
            return Type('array', element.size * count, element.align, element=element)
        if isinstance(n, c_ast.Struct):
            fields, size, align = {}, 0, 1
            for f in n.decls:
                t = self.ctype(f.type)
                size = (size + t.align - 1) // t.align * t.align
                fields[f.name] = (t, size)
                size += t.size
                align = max(align, t.align)
            return Type('struct', (size + align - 1)//align*align, align, fields=fields)
        raise AssertionError(('unsupported type AST', type(n).__name__))

    def constant(self, n):
        if isinstance(n, c_ast.Constant): return int(n.value, 0)
        if isinstance(n, c_ast.BinaryOp) and n.op == '-': return self.constant(n.left) - self.constant(n.right)
        raise AssertionError(('unsupported layout constant', type(n).__name__))

    @staticmethod
    def convert(value, typ):
        assert typ.kind in ('scalar', 'pointer')
        value &= (1 << (typ.size * 8)) - 1
        if typ.signed and value & (1 << (typ.size * 8 - 1)): value -= 1 << (typ.size * 8)
        return value

    def read(self, location):
        typ, address, local = location
        if typ.kind in ('array', 'struct'): return address
        if local:
            value = self.env[address][1]
            assert value is not None, ('uninitialized source variable', address)
            return value
        return self.convert(self.fixture.memory.get(address, typ.size), typ)

    def write(self, location, value):
        typ, address, local = location
        value = self.convert(value, typ)
        if local: self.env[address][1] = value
        else: self.fixture.memory.put(address, value, typ.size)
        return value

    def lvalue(self, n):
        if isinstance(n, c_ast.ID):
            if n.name in self.env: return self.env[n.name][0], n.name, True
            return self.globals[n.name], self.GLOBALS[n.name], False
        if isinstance(n, c_ast.ArrayRef):
            loc = self.lvalue(n.name)
            typ = loc[0]
            assert typ.kind in ('array', 'pointer')
            base = self.read(loc)
            return typ.element, base + self.expr(n.subscript) * typ.element.size, False
        if isinstance(n, c_ast.StructRef):
            loc = self.lvalue(n.name)
            typ = loc[0]
            if n.type == '->':
                assert typ.kind == 'pointer'
                base, typ = self.read(loc), typ.element
            else:
                assert n.type == '.'
                base = loc[1]
            assert typ.kind == 'struct'
            field, offset = typ.fields[n.field.name]
            return field, base + offset, False
        raise AssertionError(('unsupported lvalue', type(n).__name__))

    def expr(self, n):
        if isinstance(n, (c_ast.ID, c_ast.ArrayRef, c_ast.StructRef)): return self.read(self.lvalue(n))
        if isinstance(n, c_ast.Constant): return int(n.value, 0)
        if isinstance(n, c_ast.Cast): return self.convert(self.expr(n.expr), self.ctype(n.to_type.type))
        if isinstance(n, c_ast.UnaryOp):
            if n.op == '&': return self.lvalue(n.expr)[1]
            if n.op == '-': return -self.expr(n.expr)
            if n.op == 'p++':
                loc = self.lvalue(n.expr)
                old = self.read(loc)
                self.write(loc, old + 1)
                return old
            raise AssertionError(('unsupported unary', n.op))
        if isinstance(n, c_ast.BinaryOp):
            lhs = self.expr(n.left)
            if n.op == '||': return bool(lhs) or bool(self.expr(n.right))
            rhs = self.expr(n.right)
            if n.op == '==': return lhs == rhs
            if n.op == '!=': return lhs != rhs
            if n.op == '<': return lhs < rhs
            raise AssertionError(('unsupported binary', n.op))
        if isinstance(n, c_ast.Assignment):
            assert n.op == '='
            return self.write(self.lvalue(n.lvalue), self.expr(n.rvalue))
        if isinstance(n, c_ast.FuncCall):
            assert isinstance(n.name, c_ast.ID)
            arguments = [self.expr(a) for a in n.args.exprs] if n.args else []
            name = n.name.name
            if name in self.functions: return self.call(name, arguments)
            if name == 'sound_call_minimal':
                assert len(arguments) == 1
                self.fixture.service(0x80090254, arguments + [0, 0])
            elif name == 'model_data_load': self.fixture.service(0x8008AE8C, arguments)
            else: raise AssertionError(('unsupported call', name))
            return None
        raise AssertionError(('unsupported expression', type(n).__name__))

    def statement(self, n):
        if n is None: return
        if isinstance(n, c_ast.Compound):
            for child in n.block_items or []: self.statement(child)
        elif isinstance(n, c_ast.Decl):
            assert n.name not in self.env
            self.env[n.name] = [self.ctype(n.type), None]
            if n.init is not None: self.write(self.lvalue(c_ast.ID(n.name)), self.expr(n.init))
        elif isinstance(n, c_ast.If): self.statement(n.iftrue if self.expr(n.cond) else n.iffalse)
        elif isinstance(n, c_ast.For):
            self.statement(n.init)
            for _ in range(100):
                if not self.expr(n.cond): break
                self.statement(n.stmt)
                self.statement(n.next)
            else: raise AssertionError('source loop bound')
        elif isinstance(n, c_ast.Return):
            assert n.expr is None
            raise Returned
        else: self.expr(n)

    def call(self, name, arguments):
        function = self.functions[name]
        saved, self.env = self.env, {}
        for declaration, value in zip(function.decl.type.args.params, arguments, strict=True):
            typ = self.ctype(declaration.type)
            self.env[declaration.name] = [typ, self.convert(value, typ)]
        status = 'continue' if name == 'func_8038AA8C' else 'return'
        try: self.statement(function.body)
        except Returned: status = 'return'
        finally: self.env = saved
        return status

    def run(self, fixture, name='func_8038AA8C', raw_player=None):
        self.fixture, self.env = fixture, {}
        if name == 'func_8038AA8C': args = [fixture.descriptor, fixture.update]
        else: args = [fixture.player if raw_player is None else raw_player]
        return self.call(name, args)
