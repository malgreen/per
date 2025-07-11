# Per

Native HTTP GUI client

## Development

This project requires the Qt libraries to be installed on your system.

### Clang + clangd
If your development environment doesn't *automatically* work, you need to create a `.clangd` file in the root of the project where you point to the location of the Qt libraries, i.e.:
```
CompileFlags:
   Add: /opt/homebrew/lib/lib
```

