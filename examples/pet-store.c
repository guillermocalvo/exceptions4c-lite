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

#include <stdlib.h>
#include <exceptions4c-lite.h>

struct e4c_context exceptions4c = {0};
typedef enum pet_status { ERROR, UNKNOWN, AVAILABLE, PENDING, SOLD } pet_status;
typedef struct pet { int id; const char * name; pet_status status; } * Pet;

//! [exception_types]
const e4c_exception_type NOT_ENOUGH_MEMORY = "Not enough memory";
const e4c_exception_type PET_NOT_FOUND = "Pet not found";
//! [exception_types]

// Available pets in the store
static struct pet pets[] = {
  { .id = 0, .name = "Rocky", .status = AVAILABLE },
  { .id = 1, .name = "Garfield", .status = PENDING },
  { .id = 2, .name = "Rantanplan", .status = SOLD }
};

static void pet_free(Pet pet) { /* NOSONAR */
  // do nothing
}

static Pet pet_clone(const int id) {
  for (int index = 0; index < sizeof(pets) / sizeof(pets[0]); index++) {
    if (pets[index].id == id) {
      return &pets[index];
    }
  }
  return NULL;
}

//! [throw]
/* Returns a pet by id */
Pet pet_find(int id) {
  Pet pet = pet_clone(id);
  if (!pet) {
    THROW(PET_NOT_FOUND, "Oh no");
  }
  return pet;
}
//! [throw]

#define pet_find pet_find_throw_default
//! [throw-default]
/* Returns a pet by id */
Pet pet_find(int id) {
  Pet pet = pet_clone(id);
  if (!pet) {
    THROW(PET_NOT_FOUND, NULL);
  }
  return pet;
}
//! [throw-default]
#undef pet_find

#define pet_find pet_find_throwf
//! [throwf]
/* Returns a pet by id */
Pet pet_find(int id) {
  Pet pet = pet_clone(id);
  if (!pet) {
    THROWF(PET_NOT_FOUND, "Pet %d not found", id);
  }
  return pet;
}
//! [throwf]
#undef pet_find

#define get_pet_status get_pet_status_try
//! [try]
/* Returns the status of a pet by id */
pet_status get_pet_status(int id) {
  pet_status status = ERROR;
  TRY {
    status = pet_find(id)->status;
  }
  return status;
}
//! [try]
#undef get_pet_status

#define get_pet_status get_pet_status_catch
//! [catch]
/* Returns the status of a pet by id */
pet_status get_pet_status(int id) {
  pet_status status = ERROR;
  TRY {
    status = pet_find(id)->status;
  } CATCH (PET_NOT_FOUND) {
    status = UNKNOWN;
  } CATCH (NOT_ENOUGH_MEMORY) {
    abort();
  }
  return status;
}
//! [catch]
#undef get_pet_status

#define get_pet_status get_pet_status_catch_all
//! [catch_all]
/* Returns the status of a pet by id */
pet_status get_pet_status(int id) {
  pet_status status = ERROR;
  TRY {
    status = pet_find(id)->status;
  } CATCH_ALL {
    if (EXCEPTION.type == NOT_ENOUGH_MEMORY) {
      abort();
    }
    status = UNKNOWN;
  }
  return status;
}
//! [catch_all]
#undef get_pet_status

#define get_pet_status get_pet_status_finally
//! [finally]
/* Returns the status of a pet by id */
pet_status get_pet_status(int id) {
  pet_status status = ERROR;
  Pet pet = NULL;
  TRY {
    pet = pet_find(id);
    status = pet->status;
  } CATCH (PET_NOT_FOUND) {
    status = UNKNOWN;
  } FINALLY {
    pet_free(pet);
    pet = NULL;
    if (EXCEPTION_IS_UNCAUGHT) {
      (void) printf("get_pet_status failed! (%s)\n", EXCEPTION.message);
    }
  }
  return status;
}
//! [finally]
#undef get_pet_status

int main(void) {

  if (get_pet_status_finally(0) != AVAILABLE) {
    abort();
  }

  if (get_pet_status_finally(1) != PENDING) {
    abort();
  }

  if (get_pet_status_finally(2) != SOLD) {
    abort();
  }

  if (get_pet_status_finally(-1) != UNKNOWN) {
    abort();
  }

  return EXIT_SUCCESS;
}
