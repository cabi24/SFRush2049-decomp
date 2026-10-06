#!/bin/sh
# usage: TAG=.. spill.sh LABEL NAME  -- spill-temp / local-area layout of NAME on snapshot st_LABEL (uopt f_spilltemps):
#   AREA <uopt fn>:<line> area=N   local-area size after each write (f_readnxtinst = cfe locals, f_spilltemps = homes)
#   SPILLTEMP class= bit=          every coloured expression web f_spilltemps considers (class 1 int, 2 fp)
#   HOME bit= home= reused= size=  slot chosen per web (phase 2), CONFL bit= other= blk= block conflicts,
#   GETTEMP size= found= idx= flags=  temp-home list search;  [TMP] spill web= kind= size= temp= off=  slot per web,
#   [TMP] <fn> disp a->b  temp-area (tempdisp) growth. Full log on the builder: st_LABEL/spill.log.
. "$(dirname "$0")/lib/env.sh"
rtk $1 spill $2
