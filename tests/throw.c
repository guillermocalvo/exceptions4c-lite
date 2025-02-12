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

#include <string.h>
#include <exceptions4c-lite.h>

struct e4c_context exceptions4c = {0};
const e4c_exception_type OOPS = "Oops";

/**
 * Tests macro THROW.
 */
int main(void) {
    volatile int name1 = 0, message1 = 0, name2 = 0, message2 = 0; /* NOSONAR */

    TRY {
        THROW(OOPS, NULL);
    } CATCH_ALL {
        printf("Caught: %s: %s\n", EXCEPTION.name, EXCEPTION.message);
        name1 = strcmp(EXCEPTION.name, "OOPS") == 0;
        message1 = strcmp(EXCEPTION.message, "Oops") == 0;
    }

    TRY {
        THROW(OOPS, "I did it again");
    } CATCH_ALL {
        printf("Caught: %s: %s\n", EXCEPTION.name, EXCEPTION.message);
        name2 = strcmp(EXCEPTION.name, "OOPS") == 0;
        message2 = strcmp(EXCEPTION.message, "I did it again") == 0;
    }

    return !name1 || !message1 || !name2 || !message2;
}
