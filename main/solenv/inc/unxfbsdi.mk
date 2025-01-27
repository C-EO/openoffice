#**************************************************************
#  
#  Licensed to the Apache Software Foundation (ASF) under one
#  or more contributor license agreements.  See the NOTICE file
#  distributed with this work for additional information
#  regarding copyright ownership.  The ASF licenses this file
#  to you under the Apache License, Version 2.0 (the
#  "License"); you may not use this file except in compliance
#  with the License.  You may obtain a copy of the License at
#  
#    http://www.apache.org/licenses/LICENSE-2.0
#  
#  Unless required by applicable law or agreed to in writing,
#  software distributed under the License is distributed on an
#  "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
#  KIND, either express or implied.  See the License for the
#  specific language governing permissions and limitations
#  under the License.
#  
#**************************************************************



#
# FreeBSD/i386 specific defines
#
 
CDEFS+=-DX86

# architecture dependent flags for the C and C++ compiler that can be changed by
# exporting the variable ARCH_FLAGS="..." in the shell, which is used to start build
ARCH_FLAGS*=-mtune=pentiumpro

# Compiler flags for enabling optimizations
.IF "$(PRODUCT)"!=""
# Clang -Os optimization seems to be buggy, use -O2
.IF "$(COM)"=="CLANG"
CFLAGSOPT=-O2 -fno-unroll-loops -fno-inline -fno-strict-aliasing	# optimizing for products
.ELSE
CFLAGSOPT=-O2 -fno-unroll-loops -finline-limit=0 -fno-inline -fno-default-inline -fno-strict-aliasing		# optimizing for products
.ENDIF
.ELSE 	# "$(PRODUCT)"!=""
CFLAGSOPT= 					# no optimizing for non products
.ENDIF	# "$(PRODUCT)"!=""

# platform specific identifier for shared libs
DLLPOST=.so
DLLPOSTFIX=
