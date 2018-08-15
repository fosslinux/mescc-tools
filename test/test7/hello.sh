#! /bin/sh
## Copyright (C) 2017 Jeremiah Orians
## This file is part of stage0.
##
## stage0 is free software: you an redistribute it and/or modify
## it under the terms of the GNU General Public License as published by
## the Free Software Foundation, either version 3 of the License, or
## (at your option) any later version.
##
## stage0 is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.
##
## You should have received a copy of the GNU General Public License
## along with stage0.  If not, see <http://www.gnu.org/licenses/>.

set -x
./bin/M1 -f test/test7/hex1_amd64.M1 --LittleEndian --Architecture 2 -o test/test7/hold
./bin/hex2 -f elf_headers/elf64.hex2 -f test/test7/hold --LittleEndian --Architecture 2 --BaseAddress 0x00600000 -o test/results/test7-binary --exec_enable

if [ "$(./bin/get_machine)" = "x86_64" ]
then
	./test/results/test7-binary test/test7/hex1.hex1 > test/test7/proof
	r=$?
	[ $r = 0 ] || exit 1
	out=$(sha256sum -c test/test7/proof.answer)
	[ "$out" = "test/test7/proof: OK" ] || exit 2
fi
exit 0
