/**************************************************************
 * 
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 * 
 *   http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 * 
 *************************************************************/


/**************************************************************************
								TODO
 **************************************************************************

 *************************************************************************/

#include <sal/types.h>

namespace ftp {

class MemoryContainer {

public:

    MemoryContainer();

    ~MemoryContainer();

    int append(
        const void* pBuffer,
        size_t size,
        size_t nmemb
    ) throw();


    sal_uInt32 m_nLen,m_nWritePos;
    void *m_pBuffer;
};

}


extern "C" int memory_write(
    void *buffer,size_t size,size_t nmemb,void *stream);
