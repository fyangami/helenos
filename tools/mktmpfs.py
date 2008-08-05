#!/usr/bin/env python
#
# Copyright (c) 2008 Martin Decky
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#
# - Redistributions of source code must retain the above copyright
#   notice, this list of conditions and the following disclaimer.
# - Redistributions in binary form must reproduce the above copyright
#   notice, this list of conditions and the following disclaimer in the
#   documentation and/or other materials provided with the distribution.
# - The name of the author may not be used to endorse or promote products
#   derived from this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
# IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
# OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
# IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
# INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
# NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
# THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
# THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#
"""
TMPFS creator
"""

import sys
import os
import struct

def usage(prname):
	"Print usage syntax"
	print prname + " <ALIGNMENT> <PATH> <IMAGE>"

def recursion(root, outf):
	"Recursive directory walk"
	
	payload_size = 0
	
	for name in os.listdir(root):
		canon = os.path.join(root, name)
		
		if (os.path.isfile(canon)):
			outf.write(struct.pack("<BL" + ("%d" % len(name)) + "s", 1, len(name), name))
			payload_size += 5 + len(name)
			size = os.path.getsize(canon)
			rd = 0;
			outf.write(struct.pack("<L", size))
			payload_size += 4
			
			inf = file(canon, "r")
			while (rd < size):
				data = inf.read(4096);
				outf.write(data)
				payload_size += len(data)
				rd += len(data)
			inf.close()
		
		if (os.path.isdir(canon)):
			outf.write(struct.pack("<BL" + ("%d" % len(name)) + "s", 2, len(name), name))
			payload_size += 5 + len(name)
			payload_size += recursion(canon, outf)
			outf.write(struct.pack("<BL", 0, 0))
			payload_size += 5
	
	return payload_size

def main():
	if (len(sys.argv) < 3):
		usage(sys.argv[0])
		return
	
	path = os.path.abspath(sys.argv[1])
	if (not os.path.isdir(path)):
		print "<PATH> must be a directory"
		return
	
	outf = file(sys.argv[2], "w")
	
	outf.write(struct.pack("<5s", "TMPFS"))
	recursion(path, outf)
	outf.write(struct.pack("<BL", 0, 0))

	outf.close()
		
if __name__ == '__main__':
	main()
