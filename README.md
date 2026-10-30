# RingHandle

<div align="center">

![Platform](https://img.shields.io/badge/Platform-Windows%2010%20%7C%2011%20(x64)-0078D4?style=flat-square&logo=windows)
![Language](https://img.shields.io/badge/Language-C%2B%2B20-00599C?style=flat-square&logo=c%2B%2B)
![License](https://img.shields.io/badge/License-MIT-blue?style=flat-square)
![Build](https://img.shields.io/badge/Build-Passing-brightgreen?style=flat-square)

**Низкоуровневый инспектор таблицы дескрипторов Windows NT ядра.**  
*Прямое считывание структуры `SYSTEM_HANDLE_INFORMATION_EX` через недокументированный системный класс `SystemExtendedHandleInformation (64)`.*

</div>

---

## Архитектура и функционал

`RingHandle` считывает всю глобальную таблицу дескрипторов ядра NT без необходимости инжектить драйвер или использовать тяжелый Process Hacker:
- Обращение к `ntdll!NtQuerySystemInformation` с информационным классом `64` (`SystemExtendedHandleInformation`).
- Поддержка динамического масштабирования пула памяти для считывания более 100,000+ дескрипторов за один атомарный системный вызов.
- Вычисление физических адресов объектов ядра (`PVOID Object`), флагов прав доступа (`GrantedAccess`) и индексов типов объектов ядра.
- Фильтрация по PID процессов.

## Сборка и тесты

```powershell
g++ -std=c++20 -O2 -Iinclude src/handle_table.cpp src/nt_query.cpp tests/test_main.cpp -lntdll -o ringhandle_tests.exe
.\ringhandle_tests.exe
```

## Лицензия
MIT

<!-- verified: 2026-10-30 -->