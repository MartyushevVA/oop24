#include "dial.h"
#include "func.h"

void s_struct(const lab1::Student student, std::string title)
{
    std::cout << lab1::struct_to_string(student, title);
}

void nt_struct(const lab1::Student student, std::string title)
{
    char *nt = new char[title.length() + 1]{};
    for (std::size_t i = 0; i < title.length(); ++i)
        nt[i] = title[i];
    std::cout << lab1::struct_to_string(student, nt);
}
void ca_struct(const lab1::Student student, std::string title)
{
    char *nt = new char[title.length()];
    for (std::size_t i = 0; i < title.length(); ++i)
        nt[i] = title[i];
    std::cout << lab1::struct_to_string(student, nt, title.length());
}

void d_struct_to_string()
{
    lab1::Student student;
    std::string title;
    try
    {
        std::cout << "Введите название структуры (заголовок): ";
        std::cin >> title;
        if (!std::cin.good())
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            throw std::runtime_error("Неверный ввод, попробуйте снова.");
        }
        std::cout << "Введите имя: ";
        std::cin >> student.name;
        if (!std::cin.good())
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            throw std::runtime_error("Неверный ввод, попробуйте снова.");
        }
        std::cout << "Введите группу: ";
        std::cin >> student.group;
        if (!std::cin.good())
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            throw std::runtime_error("Неверный ввод, попробуйте снова.");
        }
        std::cout << "Введите оценку: ";
        std::cin >> student.grade;
        if (!std::cin.good())
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            throw std::runtime_error("Неверный ввод, попробуйте снова.");
        }
    }
    catch (const std::runtime_error &e)
    {
        std::cout << e.what() << std::endl;
    }
    catch (...)
    {
        throw;
    }
    int option = 0;
    while (true)
    {
        std::cout << "Выберите вариант перегрузки функции (конвертация заголовка): " << std::endl
                  << "1) Нуль-терминированная строка" << std::endl
                  << "2) Массив символов и количество" << std::endl
                  << "3) Экземляр класса string" << std::endl
                  << "4) Выход" << std::endl;
        std::cin >> option;
        try
        {
            if (!std::cin.good())
            {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                throw std::runtime_error("Неверный ввод, попробуйте снова.");
            }
            switch (option)
            {
            case 1:
                nt_struct(student, title);
                return;
            case 2:
                ca_struct(student, title);
                return;
            case 3:
                s_struct(student, title);
                return;
            case 4:
                std::cout << "Выход из программы." << std::endl;
                throw 0;
            default:
                throw std::runtime_error("Число вне допустимого диапазона значений. Повторите ввод.");
            }
        }
        catch (const std::runtime_error &e)
        {
            std::cout << e.what() << std::endl;
        }
        catch (...)
        {
            throw;
        }
    }
}

void s_string(const std::string java_prop)
{
    lab1::Student student = lab1::string_to_struct(java_prop);
    std::cout << "{name=" << student.name << ", group=" << student.group << ", grade=" << student.grade << "};" << std::endl;
}

void nt_string(const std::string java_prop)
{
    char *njp = new char[java_prop.length() + 1]{};
    for (std::size_t i = 0; i < java_prop.length(); ++i)
        njp[i] = java_prop[i];
    lab1::Student student = lab1::string_to_struct(njp);
    std::cout << "{name=" << student.name << ", group=" << student.group << ", grade=" << student.grade << "};" << std::endl;
}

void ca_string(const std::string java_prop)
{
    char *njp = new char[java_prop.length()];
    for (std::size_t i = 0; i < java_prop.length(); ++i)
        njp[i] = java_prop[i];
    lab1::Student student = lab1::string_to_struct(njp, java_prop.length());
    std::cout << "{name=" << student.name << ", group=" << student.group << ", grade=" << student.grade << "};" << std::endl;
}

void d_string_to_struct()
{
    std::string java_prop;
    std::cout << "Введите java prop строку: ";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    getline(std::cin, java_prop);
    int option = 0;
    while (true)
    {
        std::cout << "Выберите вариант перегрузки функции: " << std::endl
                  << "1) Нуль-терминированная строка" << std::endl
                  << "2) Массив символов и количество" << std::endl
                  << "3) Экземляр класса string" << std::endl
                  << "4) Выход" << std::endl;
        std::cin >> option;
        try
        {
            if (!std::cin.good())
            {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                throw std::runtime_error("Неверный ввод, попробуйте снова.");
            }
            switch (option)
            {
            case 1:
                nt_string(java_prop);
                return;
            case 2:
                ca_string(java_prop);
                return;
            case 3:
                s_string(java_prop);
                return;
            case 4:
                throw 0;
            default:
                throw std::runtime_error("Число вне допустимого диапазона значений. Повторите ввод.");
            }
        }
        catch (const std::runtime_error &e)
        {
            std::cout << e.what() << std::endl;
        }
        catch (...)
        {
            throw;
        }
    }
}

void dl::dialog()
{
    int option = 0;
    while (true)
    {
        std::cout << "Выберите опцию: " << std::endl
                  << "1) Структура --> java properties" << std::endl
                  << "2) Java properties --> структура" << std::endl
                  << "3) Выход" << std::endl;
        std::cin >> option;
        try
        {
            if (!std::cin.good())
            {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                throw std::runtime_error("Неверный ввод, попробуйте снова.");
            }
            switch (option)
            {
            case 1:
                d_struct_to_string();
                break;
            case 2:
                d_string_to_struct();
                break;
            case 3:
                return;
            default:
                throw std::runtime_error("Число вне допустимого диапазона значений. Повторите ввод.");
            }
        }
        catch (const std::runtime_error &e)
        {
            std::cout << e.what() << std::endl;
        }
        catch (const std::bad_alloc &ba)
        {
            std::cerr << "Недостаточно памяти" << std::endl;
        }
        catch (const int)
        {
            return;
        }
        catch (...)
        {
        }
    }
}