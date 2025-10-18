# Xenoblade Chronicles SB Container Format

This document consolidates what the tool currently understands about Xenoblade Chronicles (Wii) `.sb` script containers. The
information is distilled from the reader and writer implemented in this repository.

## File header

* The file starts with the four byte ASCII signature `SB  ` (`0x53 0x42 0x20 0x20`).
* Immediately following the signature are a version byte, a reserved byte, the script flags, and an `isLoaded` byte. These fields
  are surfaced while parsing a script file so they can be preserved when recompiling the same script.【F:Xenoblade 1 Script Tool/Script.cpp†L78-L109】
* Thirteen 32-bit little-endian offsets follow, pointing to the ordered sections: code, ID pool, integer pool, fixed pool, string
  pool, function pool, plugin imports, OC imports, function imports, static variables, local pool, system attribute pool, and user
  attribute pool. A fourteenth offset optionally points to debug information. These offsets always reference absolute locations in
  the file, with the first section normally beginning at offset `0x40` (the size of the fixed header).【F:Xenoblade 1 Script Tool/Script.cpp†L110-L158】【F:Xenoblade 1 Script Tool/Script.cpp†L214-L247】

## Sections

Each section is serialized as a self-describing blob that begins with a small header. The loader keeps the raw bytes of every
section so unchanged data can be copied back into rebuilt archives.【F:Xenoblade 1 Script Tool/Script.cpp†L158-L206】

When translating, invoke the compiler with `--strings-only` and a reference SB file to reuse every section except the string pool
verbatim. This preserves the camera and scripting metadata that the game expects while letting text edits shrink or grow freely.【F:Xenoblade 1 Script Tool/Script.cpp†L1066-L1152】

### Code

* The code section header stores a 32-bit offset to the bytecode (`0x0C`), a reserved 32-bit field, and a 32-bit size that counts the
  unpadded bytecode length. Functions are serialized sequentially with sentinel spacer bytes (`0x5E` for `_main_`, `0x4B` for the
  other functions).【F:Xenoblade 1 Script Tool/Script.cpp†L1198-L1232】
* Instructions are written as opcode bytes followed by operand data. `SWITCH` instructions also serialize a default branch target
  and case table. The code generator recreates this layout from the parsed instruction objects.【F:Xenoblade 1 Script Tool/Instruction.cpp†L49-L93】

### String-like pools

* The ID pool and string pool start with headers that record their element counts, the size of the offset table entries, and the
  offset from the section start to the data payload. String data is stored as Shift-JIS byte sequences with null terminators. The
  pool writer re-encrypts four-byte blocks using the `encryptBytes` permutation so the data matches the original encoding method
  used by the game.【F:Xenoblade 1 Script Tool/Script.cpp†L1274-L1314】【F:Xenoblade 1 Script Tool/Script.cpp†L920-L942】

### Numeric pools

* Integer and fixed pools contain arrays of 32-bit values preceded by headers that store the element count and the offset from the
  section start to the first value (`0x08`). Floating-point values are written as IEEE 754 32-bit numbers, respecting the source
  endianness flag.【F:Xenoblade 1 Script Tool/Script.cpp†L1234-L1269】

### Function metadata

* The function table references entries in the ID pool, stores argument counts and metadata (`field4`, `field6`, `field10`), and
  records the start/end offsets into the bytecode. When rebuilding the local pool, new entries are assigned sequential indices so
  cross references stay valid.【F:Xenoblade 1 Script Tool/Script.cpp†L708-L752】【F:Xenoblade 1 Script Tool/Script.cpp†L1405-L1437】

### Imports and attributes

* Plugin imports and OC imports reuse ID pool references to describe external dependencies. System and user attribute pools
  similarly point back into the ID pool, with `0xFFFF` denoting empty slots.【F:Xenoblade 1 Script Tool/Script.cpp†L752-L846】

### Local pools and static variables

* Local variable lists and script-level static variables are serialized as arrays of objects. Each object encodes its type, array
  flag, length, value, and (for 64-bit scripts) an optional field used as pointer storage space.【F:Xenoblade 1 Script Tool/Script.cpp†L820-L903】【F:Xenoblade 1 Script Tool/Script.cpp†L1379-L1404】

## Padding

* The original alignment of the file is detected while loading by searching for the largest power-of-two divisor of the original
  file size. Rebuilt scripts pad only to the detected alignment, so translated text that shortens the string pool can shrink the
  archive without introducing stray zero-filled tails.【F:Xenoblade 1 Script Tool/Script.cpp†L132-L158】【F:Xenoblade 1 Script Tool/Script.cpp†L1182-L1185】

