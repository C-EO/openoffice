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



#ifndef _OSL_CONDITN_HXX_
#define _OSL_CONDITN_HXX_

#ifdef __cplusplus

#include <osl/time.h>

#include <osl/conditn.h>


namespace osl
{     

	class Condition
	{
	public:

		enum Result
		{
			result_ok      = osl_cond_result_ok,
			result_error   = osl_cond_result_error,
			result_timeout = osl_cond_result_timeout
		};

        /* Create a condition.
		 */
        Condition()
		{
			condition = osl_createCondition();
		}

        /* Release the OS-structures and free condition data-structure.
         */
		~Condition()
		{
			osl_destroyCondition(condition);
		}

        /* Release all waiting threads, check returns sal_True.
         */
        void set()
		{
			osl_setCondition(condition);
		}

		/* Reset condition to false: wait() will block, check() returns sal_False.
         */
		void reset() {
			osl_resetCondition(condition);
		}

		/** Blocks the calling thread until condition is set.
		 */
		Result wait(const TimeValue *pTimeout = 0)
		{
			return (Result) osl_waitCondition(condition, pTimeout);
		}

		/** Checks if the condition is set without blocking.
		 */
		sal_Bool check()
		{
			return osl_checkCondition(condition);
		}


	private:
		oslCondition condition;

        /** The underlying oslCondition has no reference count.
        
        Since the underlying oslCondition is not a reference counted object, copy
        constructed Condition may work on an already destructed oslCondition object.
        
        */
        Condition(const Condition&);
        
        /** The underlying oslCondition has no reference count.
        
        When destructed, the Condition object destroys the undelying oslCondition,
        which might cause severe problems in case it's a temporary object.
         
        */
        Condition(oslCondition condition);
        
        /** This assignment operator is private for the same reason as
            the copy constructor.
        */
        Condition& operator= (const Condition&);
        
        /** This assignment operator is private for the same reason as
            the constructor taking a oslCondition argument.
        */
        Condition& operator= (oslCondition);
	};

}

#endif  /* __cplusplus */
#endif	/* _OSL_CONDITN_HXX_ */
