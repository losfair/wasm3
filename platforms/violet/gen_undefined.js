const fs = require("fs");

const list = fs.readFileSync("./undefined_list.txt", "utf-8")
  .split("\n")
  .map(x => x.trim())
  .filter(x => x)
  .map(x => x.split("undefined symbol: ")[1]);

console.log("void __attribute__((noreturn)) __fatal_error(const char *);\n");

for(const elem of list) {
  console.log(`void ${elem}() { __fatal_error("not implemented: ${elem}"); }`)
}
