//
//  Wasm3 - high performance WebAssembly interpreter written in C.
//
//  Copyright © 2019 Steven Massey, Volodymyr Shymanskyy.
//  All rights reserved.
//

#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include "wasm3.h"
#include "m3_config.h"

#include "extra/fib32.wasm.h"

#define FATAL(msg, ...) { printf("Fatal: " msg "\n", ##__VA_ARGS__); return; }

volatile uint32_t *time_low = (volatile uint32_t *) 0xfe000010;
volatile uint32_t *time_high = (volatile uint32_t *) 0xfe000014;
volatile uint32_t *putchar_port = (volatile uint32_t *) 0xfe000000;
volatile uint32_t *terminate_port = (volatile uint32_t *) 0xfe000004;

void run_wasm()
{
    M3Result result = m3Err_none;

    uint8_t* wasm = (uint8_t*)fib32_wasm;
    uint32_t fsize = fib32_wasm_len;

    printf("Loading WebAssembly...\n");
    IM3Environment env = m3_NewEnvironment ();
    if (!env) FATAL("m3_NewEnvironment failed");

    IM3Runtime runtime = m3_NewRuntime (env, 1024, NULL);
    if (!runtime) FATAL("m3_NewRuntime failed");

    IM3Module module;
    result = m3_ParseModule (env, &module, wasm, fsize);
    if (result) FATAL("m3_ParseModule: %s", result);

    result = m3_LoadModule (runtime, module);
    if (result) FATAL("m3_LoadModule: %s", result);

    IM3Function f;
    result = m3_FindFunction (&f, runtime, "fib");
    if (result) FATAL("m3_FindFunction: %s", result);

    printf("Running...\n");

    result = m3_CallV (f, 24);
    if (result) FATAL("m3_Call: %s", result);

    uint32_t value = 0;
    result = m3_GetResultsV (f, &value);
    if (result) FATAL("m3_GetResults: %s", result);

    printf("Result: %d\n", value);
}

int main() {
    printf("\n");
    printf("Wasm3 v" M3_VERSION " on HiFive1 (" M3_ARCH "), build " __DATE__ " " __TIME__ "\n");
    // TODO: fix clock (shows wrong time)
    clock_t start = clock();
    run_wasm();
    clock_t end = clock();

    printf("Elapsed: %ld ms\n", (end - start)*1000 / CLOCKS_PER_SEC);
    while(1) {}
}

void _putchar(char c) {
    *putchar_port = (uint32_t) c;
}

void __attribute__((noreturn)) __fatal_error(const char *msg) {
  for(; *msg; msg++) _putchar(*msg);
  _putchar('\n');
  *terminate_port = 1;
  while(1) {}
}