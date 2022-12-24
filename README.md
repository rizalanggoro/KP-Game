## Persiapan MinGW

- Install MinGW Builds 7.3.0 (64-bit) melalui link [berikut.](https://sourceforge.net/projects/mingw-w64/files/Toolchains%20targetting%20Win64/Personal%20Builds/mingw-builds/7.3.0/threads-posix/seh/x86_64-7.3.0-release-posix-seh-rt_v5-rev0.7z/download) (Selain versi 7.3.0 tidak work).
- Ekstrak file `.7z` menggunakan winrar atau sejenisnya, kemudian pindahkan ke folder `C:/` atau lainnya.
- Tambahkan `path_ke_folder_mingw-7.3.0/bin` ke `Environment Variables` pada bagian `Path`.

  Contoh : `D:\application\mingw64\bin`

- Untuk bisa mengeksekusi file `Makefile`, lakukan rename pada file `path_ke_folder_mingw-7.3.0/bin/mingw32-make.exe` diubah menjadi `make.exe`.

## Persiapan VSCode

- Tambahkan `path_ke_folder_sfml/include` ke `.vscode/c_cpp_properties.json` pada bagian `includePath`.

  Contoh :<br>

  ```json
  "includePath": [
    "${workspaceFolder}/**", "D:\\sdk\\SFML-gcc\\include",
  ]
  ```

- Buka file `Makefile`, ubah `SFML_DIR` sesuai dengan folder instalasi SFML.
- Ubah `CLEAR_TERMINAL` sesuai dengan profile terminal yang digunakan, (`cls` atau `clear`)

## Cara menjalankan program

- Ketikkan perintah `make` pada terminal.

## Struktur Folder

- root
  - assets
    - Berisi asset game seperti font, gambar, suara dll.
    - fonts
      - `nama-font.ttf`
      - `nama-font-2.ttf`
    - images
      - `gambar.png`
      - `gambar-2.png`
  - headers
    - Berisi file-file header `*.hpp` yang kemudian bisa di include pada file `main.cpp` dll.
    - `button.hpp`
    - `background.hpp`
  - sources
    - Berisi file-file source `*.cpp` yang terdapat penjabaran berbagai fungsi dari header file.
    - `button.cpp`
    - `background.cpp`
  - `main.cpp`
  - `main.exe`
  - `*.dll`

## Error

Program tidak bisa dijalankan? Pastikan telah menambahkan semua file `*.dll` yang terdapat pada `path_ke_folder_sfml/bin` ke folder root project.

## Shortcut VS Code

- Klik `Ctrl Shift P` lalu ketikkan `shortcut`, pilih `Preferences: Open Keyboard Shortcuts (JSON)`. Maka akan terbuka file `keybindings.json`.
- Untuk menggunakan shortcut `Ctrl Alt X` sebagai shortcut run, tambahkan kode berikut.

```json
// ...code lainnya
{
  "key": "ctrl+alt+x",
  "command": "workbench.action.terminal.sendSequence",
  "args": {
    "text": "make\u000D"
  }
}
```

- Simpan perubahan, lalu tutup file `keybindings.json`.
- Untuk melakukan run program cukup menekan `Ctrl Alt X` secara bersamaan.
