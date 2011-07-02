# FreeRDP: A Remote Desktop Protocol Client
# cunit cmake build script
#
# Copyright 2011 O.S. Systems Software Ltda.
# Copyright 2011 Otavio Salvador <otavio@ossystems.com.br>
# Copyright 2011 Marc-Andre Moreau <marcandre.moreau@gmail.com>
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

include_directories(.)
include_directories(../include)
include_directories(../libfreerdp-gdi)

add_executable(test_freerdp
	test_color.c
	test_color.h
	test_libgdi.c
	test_libgdi.h
	test_stream.c
	test_stream.h
	test_freerdp.c
	test_freerdp.h)

target_link_libraries(test_freerdp cunit)

target_link_libraries(test_freerdp freerdp-gdi)
target_link_libraries(test_freerdp freerdp-asn1)
target_link_libraries(test_freerdp freerdp-utils)
