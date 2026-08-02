#!/bin/bash

# Проверяем, запущен ли скрипт от root
if [ "$EUID" -ne 0 ]; then
  echo "❌ Пожалуйста, запустите скрипт с правами sudo"
  exit 1
fi

# Функция, которая выполнится при нажатии Ctrl+C
cleanup() {
    echo ""
    echo "=== Завершение мониторинга ==="
    echo "Выгружаю модуль..."
    sudo rmmod kernel_timer_demo
    echo "Последние сообщения:"
    dmesg | tail -n 1
    exit 0
}

# Регистрируем функцию на сигнал SIGINT (Ctrl+C)
trap cleanup SIGINT

# Очистка буфера
dmesg -C

# Загрузка модуля
insmod kernel_timer_demo.ko

echo "Мониторинг запущен. Нажмите Ctrl+C для завершения..."

# Результат
dmesg -wT

# rmmod kernel_timer_demo
# dmesg | tail -n 1
