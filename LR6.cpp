#include <iostream>
#include <cmath>
#include <ctime>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>



struct Student
{
    std::string FSM;
    struct Discipline
    {
        std::string Subject;
        int Mark;
    };
    Discipline Stats[6];
};

bool comp(const Student &Group1, const Student &Group2) //Компаратор для std::sort
{
    return Group1.FSM < Group2.FSM;
}

void PrintInfo(std::vector<Student> &Vec) 
{
    for (Student &St : Vec)
    {
        std::cout << "Студент: " << St.FSM << std::endl;
        for (int i = 0; i < 6; i++)
        {
            std::cout << "Дисциплина: " << St.Stats[i].Subject << std::endl;
            std::cout << "Оценка: " << St.Stats[i].Mark << std::endl;
        }
        std::cout << std::endl;
    }
}

std::ofstream &operator<<(std::ofstream &file, std::vector<Student> &Vec) //Запись в файл
{
    for (Student& St : Vec)
    {
        file << "Студент: " << St.FSM << std::endl;
        for (int i = 0; i < 6; i++)
        {
            file << "Дисциплина: " << St.Stats[i].Subject << std::endl;
            file << "Оценка: " << St.Stats[i].Mark << std::endl;
        }
    }
    return file;
}

std::ifstream &operator>>(std::ifstream &file, std::vector<Student> &Vec) //Чтение из файла
{
    Student St;
    std::string buff;
    while (!file.eof())
    {
        file.ignore(9);
        std::getline(file, St.FSM);
        for (int i = 0; i < 6; i++)
        {
            file.ignore(12);
            std::getline(file, St.Stats[i].Subject);
            file.ignore(8);
            std::getline(file, buff);
            St.Stats[i].Mark = std::stoi(buff);
        }
        Vec.push_back(St);
    }
    Vec.pop_back();
    return file;
}

void ReadFromFile(std::string Name, std::vector<Student> &Vec)
{
    std::ifstream file;
    file.open(Name);
    file >> Vec;
    file.close();
}

void WriteToFile(std::string Name, std::vector<Student> &Vec)
{
    std::ofstream file;
    file.open(Name);
    file << Vec;
    file.close();
}

void ReadFromFileBin(std::string Name, std::vector<Student> &Vec)
{
    unsigned int buffer = 0;
    std::ifstream file(Name, std::ios::binary);
    file.read(reinterpret_cast<char*>(&buffer), sizeof(unsigned int));
    Vec.resize(buffer);
    for (Student &St : Vec)
    {
        file.read(reinterpret_cast<char*>(&buffer), sizeof(unsigned int));
        St.FSM.resize(buffer); 
        file.read(const_cast<char*>(St.FSM.data()), buffer);
        for (int i = 0; i < 6; i++)
        {
            file.read(reinterpret_cast<char*>(&buffer), sizeof(unsigned int));
            St.Stats[i].Subject.resize(buffer);
            file.read(const_cast<char*>(St.Stats[i].Subject.data()), buffer);
            file.read(reinterpret_cast<char*>(&St.Stats[i].Mark), sizeof(int));
        }
    }
}

void WriteToFileBin(std::string Name, std::vector<Student> &Vec)
{
    unsigned int buffer = Vec.size();
    std::ofstream file(Name, std::ios::binary);
    file.write(reinterpret_cast<char*>(&buffer), sizeof(unsigned int));
    for (Student &St : Vec)
    {
        buffer = St.FSM.length();
        file.write(reinterpret_cast<char*>(&buffer), sizeof(unsigned int));
        file.write(St.FSM.data(), buffer);
        for (int i = 0; i < 6; i++)
        {
            buffer = St.Stats[i].Subject.length();
            file.write(reinterpret_cast<char*>(&buffer), sizeof(unsigned int));
            file.write(St.Stats[i].Subject.data(), buffer);
            file.write(reinterpret_cast<char*>(&St.Stats[i].Mark), sizeof(int));
        }
    }
}

int main()
{
    setlocale(LC_ALL, "Russian");
    srand(time(NULL));
    std::vector<Student> Check1;
    std::vector<Student> Check2;
    std::vector<std::string> SubjectData = { "Алгебра", "Матанализ", "Физика", "История", "Психология", "Инжграф", "Информатика", "Политология", "Физкультура", "Экономика" };
    std::vector<std::string> StudentData = { "Кожевникова Ника Ипполитовна", "Счастливцева Дарья Всеволодовна", "Матвеева Изольда Павеловна", "Шерстова Татьяна Давидовна", "Невшупа Яна Данилевна", "Колесников Андрей Денисович", "Курдиков Кирилл Олегович", "Ямлиханов Рубен Елисеевич", "Нагиев Моисей Назарович", "Лобза Валерьян Ипатиевич" };
    int x = 0;
    int k = 0;
    int N = 0;
    bool isFailed;
    std::cout << "Введите количество студентов(до 10 включительно): ";
    std::cin >> N;
    std::cout << std::endl;
    std::vector<Student> Group(N);
    for (Student &St : Group)
    {
        
        x = rand() % (10 - k);
        St.FSM = StudentData[x];
        StudentData.erase(StudentData.begin() + x);
        k++;
        /*std::cout << "Введите ФИО студента: ";
        std::getline(std::cin, Student.FSM);*/
        for (int i = 0; i < 6; i++)
        {
            St.Stats[i].Subject = SubjectData[i];
            St.Stats[i].Mark = (rand() % 4) + 2;
        }
    }


    std::sort(Group.begin(), Group.end(), comp);
    std::cout << "----Отсортировано----" << std::endl;
    for (Student &St : Group)
    {
        std::cout << St.FSM << std::endl;
    }
    std::cout << std::endl;


    std::cout << "----Студенты с хотя бы одним неуд----" << std::endl;
    k = 0;
    for (Student &St : Group)
    {
        isFailed = false;
        for (int i = 0; i < 6; i++)
        {
            if (St.Stats[i].Mark == 2) isFailed = true;
        }
        if (isFailed)
        {
            k++;
            std::cout << k << "." << St.FSM << std::endl;
        }
    }
    std::cout << "Всего студентов с неуд: " << k << std::endl;
    std::cout << std::endl;


    std::cout << "----Проверка----" << std::endl;
    PrintInfo(Group);
    std::cout << std::endl;

    std::cout << "----Проверка чтения и записи----" << std::endl;
    WriteToFile("Hello1.txt", Group);
    ReadFromFile("Hello1.txt", Check1);
    PrintInfo(Check1);
    std::cout << std::endl << std::endl;


    std::cout << "----Проверка чтения и записи в бинарном режиме----" << std::endl;
    WriteToFileBin("Hello2.txt", Group);
    ReadFromFileBin("Hello2.txt", Check2);
    PrintInfo(Check2);
    std::cout << std::endl << std::endl;
    return 0;
}