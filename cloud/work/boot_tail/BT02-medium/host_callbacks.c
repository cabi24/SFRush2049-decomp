/* Host-only callback behavior tests; never a matching compilation unit. */
#include <assert.h>
#include <string.h>

void func_8001144C(void);
void func_80011848(void);
void func_80011D24(void);
void func_800121BC(void);
void func_8001261C(void);
void func_800140F8(void);
void func_80014140(void);
void func_80013DEC(void) {}

void (*D_8003801C)(void *);
void *D_800382D8, *D_800382DC, *D_80038298, *D_800382E4;
void *D_8003833C, *D_80038338, *D_80038350, *D_8003834C;
volatile unsigned char D_800382CC;
void *D_8003802C;
unsigned short D_80038028;
unsigned char D_800382B0[24];
void (*D_80038010)(void *, unsigned short, void *);
unsigned short D_80038360, D_80038292;
volatile short D_80038362;
void (*D_80038020)(void), (*D_80038024)(void);
void (*D_80038008)(void (*)(void));
void (*D_8003800C)(void (*)(void));

static int tokens[8], nfreed, ncalls, nregistered;
static void *freed[8], *received_buffer, *received_queue;
static unsigned short received_count;
static void (*registered_callback)(void);
static void release(void *p) { freed[nfreed++] = p; }
static void submit(void *p, unsigned short n, void *q)
{
    assert(D_800382CC == 1);
    ++ncalls;
    received_buffer = p;
    received_count = n;
    received_queue = q;
}
static void register_callback(void (*cb)(void))
{
    ++nregistered;
    registered_callback = cb;
}

int main(int argc, char **argv)
{
    int i;
    D_8003801C = release;
    D_80038008 = register_callback;
    D_8003800C = register_callback;
    if (argc == 2 && strcmp(argv[1], "busy8") == 0) {
        D_800382CC = 1;
        func_80011848();
        return 3;
    }
    if (argc == 2 && strcmp(argv[1], "busy16") == 0) {
        D_80038292 = 1;
        func_80014140();
        return 3;
    }
    D_800382D8 = &tokens[0]; D_800382DC = &tokens[1];
    D_80038298 = &tokens[2]; D_800382E4 = &tokens[3];
    func_8001144C();
    assert(nfreed == 4);
    for (i = 0; i < 4; ++i) assert(freed[i] == &tokens[i]);
    nfreed = 0;
    D_8003833C = &tokens[4]; D_80038338 = &tokens[5];
    func_800121BC();
    assert(nfreed == 2 && freed[0] == &tokens[4] && freed[1] == &tokens[5]);
    nfreed = 0;
    D_80038350 = &tokens[6]; D_8003834C = &tokens[7];
    func_8001261C();
    assert(nfreed == 2 && freed[0] == &tokens[6] && freed[1] == &tokens[7]);
    nfreed = 0;
    D_8003802C = &tokens[0]; D_800382CC = 0;
    func_80011848();
    assert(nfreed == 1 && freed[0] == &tokens[0] && D_800382CC == 0);
    D_80038010 = submit;
    D_80038028 = 0; D_800382CC = 7;
    func_80011D24();
    assert(ncalls == 0 && D_800382CC == 7);
    D_80038028 = 1;
    func_80011D24();
    assert(ncalls == 1 && received_count == 1);
    assert(received_buffer == D_8003802C && received_queue == D_800382B0);
    D_80038028 = 65535;
    func_80011D24();
    assert(ncalls == 2 && received_count == 65535);
    D_80038020 = func_80013DEC; D_80038024 = func_80013DEC;
    func_800140F8();
    assert(D_80038360 == 65535 && !D_80038020 && !D_80038024);
    assert(nregistered == 1 && registered_callback == func_80013DEC);
    D_80038292 = 0;
    func_80014140();
    assert(D_80038362 == 0 && nregistered == 2);
    D_80038292 = 65535;
    func_80014140();
    assert(D_80038362 == -1 && nregistered == 3);
    assert(registered_callback == func_80013DEC);
    return 0;
}
