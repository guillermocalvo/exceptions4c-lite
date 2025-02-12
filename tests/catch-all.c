/*
 * Copyright 2025 Guillermo Calvo
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <exceptions4c-lite.h>

struct e4c_context exceptions4c = {0};
const e4c_exception_type OOPS = "Oops";

/**
 * Tests macro CATCH_ALL.
 */
int main(void) {
    volatile int caught = 0; /* NOSONAR */

    TRY {
        THROW(OOPS, NULL);
    } CATCH_ALL {
        caught = 1;
        printf("Caught: %s: %s\n", EXCEPTION.name, EXCEPTION.message);
    }

    return !caught;
}
