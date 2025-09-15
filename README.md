# Console Tetris (Arduino Project Adaptation) / Консольний Tetris (адаптація Arduino-проєкту)

## English

This project is an adaptation of a student Arduino project turned into a **console Tetris game**.  
The program runs on **Windows** and **Linux** without changing the main code, thanks to the compatibility layer `my_conio.h`.

### Features
- **Cross-platform:** `tetr.cpp` compiles on Windows (using `<conio.h>`) and on Linux (via `my_conio.h`).
- **Colors:** By default the game is black and white. The macro `#define COLORS` enables color mode.
- **Console Palette Preview:** The macro `//#define PRESENT_COLORS` shows the full color palette available in your console.
- **Color Filtering:** The variables `ban_color[]` and `mask_color` allow filtering out unwanted colors or applying a color mask.

### Files
- **`tetr.cpp`** – main game logic.  
- **`my_conio.h`** – cross-platform library that intercepts Windows API calls (`<conio.h>`, `<Windows.h>`) and converts them into Linux ANSI escape sequences.  
- **`tetr.h`** – (if present) defines the game field dimensions and constants.

### Controls
- **W / ↑** – rotate piece  
- **A / ←** – move left  
- **D / →** – move right  
- **S / ↓** – drop faster  

### Compilation and Run

#### Windows (MinGW / MSVC)
```bash
g++ tetr.cpp -o tetr.exe
tetr.exe
```

#### Linux (g++)
```bash
g++ tetr.cpp -o tetr -std=c++11
./tetr
```

> **Note:**
> On Linux, ensure `my_conio.h` is in the same directory as `tetr.cpp`.

### Color Settings
- `#define COLORS` – enables color mode.
- `#define PRESENT_COLORS` – uncomment to display your console's palette.
- `ban_color[]` – list of forbidden colors.
- `mask_color` – bitmask for allowed colors.

### Screenshots
| Windows | Linux (via SSH) |
|---------|----------------|
| ![Windows Screenshot](screenshot_win.png) | ![Linux Screenshot](screenshot_linux.png) |

## Українська

Цей проєкт — адаптація студентського проєкту для Arduino у вигляді консольної гри Tetris.

Програма працює у Windows та Linux без зміни основного коду, завдяки використанню сумісного шару `my_conio.h`.

### Особливості
- **Кросплатформність:** `tetr.cpp` компілюється під Windows (з `<conio.h>`) та під Linux (через `my_conio.h`).
- **Кольори:** За замовчуванням гра чорно-біла. Макрос `#define COLORS` вмикає кольоровий режим.
- **Перегляд палітри консолі:** Макрос `//#define PRESENT_COLORS` показує повну палітру кольорів вашої консолі.
- **Фільтрація кольорів:** Змінні `ban_color[]` та `mask_color` дозволяють виключати небажані кольори або застосовувати маскування.

### Файли
- **`tetr.cpp`** – основний код гри.
- **`my_conio.h`** – кросплатформна бібліотека, яка перехоплює Windows-виклики (API `<conio.h>`, `<Windows.h>`) і транслює їх у Linux-виклики ANSI escape-кодами.
- **`tetr.h`** – визначає розміри ігрового поля та константи.

### Керування
- **W / ↑** – обертання фігури
- **A / ←** – рух ліворуч
- **D / →** – рух праворуч
- **S / ↓** – швидке падіння

### Компіляція та запуск

#### Windows (MinGW / MSVC)
```bash
g++ tetr.cpp -o tetr.exe
tetr.exe
```

#### Linux (g++)
```bash
g++ tetr.cpp -o tetr -std=c++11
./tetr
```

> **Примітка:**
> У Linux обов'язково наявний файл `my_conio.h` у тій же папці, що й `tetr.cpp`.

### Налаштування кольорів
- `#define COLORS` – вмикає кольоровий режим.
- `#define PRESENT_COLORS` – розкоментуйте, щоб переглянути палітру вашої консолі.
- `ban_color[]` – список кольорів, які не будуть використані.
- `mask_color` – бітова маска дозволених кольорів.

### Скріншоти
| Windows | Linux (через SSH) |
|---------|------------------|
| ![Скріншот Windows](screenshot_win.png) | ![Скриншот Linux](screenshot_linux.png) |

### Ліцензія / License
Проєкт поширюється як є. Ви можете змінювати, адаптовувати чи інтегрувати його у власні навчальні або хобі-проєкти.

This project is provided as-is. You are free to modify, adapt, or integrate it into your own learning or hobby projects.
