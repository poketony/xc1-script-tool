# xc1-script-tool
Tool for editing script (SB) files for Xenoblade Chronicles (Wii)

## Usage

```
xc1-script-tool decompile <input_script.sb> <output_csv>
xc1-script-tool compile <input_csv> <output_script> [original_script] [--strings-only]
```

Pass the original script when recompiling to preserve untouched sections and keep padding consistent with the base game.  Use
`--strings-only` to rebuild only the string section (suitable for pure translation work) while byte-for-byte copying every other
section from the supplied original file.
