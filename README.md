# NativeEXE 1

*Under Dev*

Windows as DOS (Style)

No more fucking `win32k.sys` or `csrss.exe`

JUST `ntoskrnl.exe` (and necessary drv)

`ntdll.dll` is all you need

# Features

- Open `\Device\KeyboardClass0` for keyboard input
- Read keyboard input

# Todo

- [x] Deal with Keyboard Scancodes
- [x] Make a mini-shell
- [ ] Filesystem support
- [ ] Create Process support
- [ ] STDIO redirection

# Now Achieved

![screenshot](./screenshot/5.png)

See [Gallery](./screenshot) for more screenshots.

See [research.md](research.md) for how I debugged this.

# Bugs

~~### `\Device\KeyboardClass0` Open failed~~

~~### read char failed~~