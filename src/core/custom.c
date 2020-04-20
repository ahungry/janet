/*
* Copyright (c) 2020 Calvin Rose
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to
* deal in the Software without restriction, including without limitation the
* rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
* sell copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
* FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
* IN THE SOFTWARE.
*/

#ifndef JANET_AMALG
#include "features.h"
#include <janet.h>
#include "gc.h"
#include "util.h"
#include "state.h"
#endif

#include "/home/mcarter/src/janet-code/target/super/curl_wrap_app.c"
#include "/home/mcarter/src/janet-code/target/super/iup_wrap.c"
#include "/home/mcarter/src/janet-code/target/super/circlet/mongoose.c"
#include "/home/mcarter/src/janet-code/target/super/circlet/circlet.c"

/* Module entry point */
void janet_lib_custom(JanetTable *env) {
    janet_core_cfuns(env, NULL, circlet_cfuns);
    janet_core_cfuns(env, NULL, curl_cfuns);
    janet_core_cfuns(env, NULL, cfuns);
}
