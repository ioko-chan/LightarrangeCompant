#include <map>
#include <iostream>
#include <fstream>
#include <string>
#include <set>
//методы вместо операторов 
//Должность
class Job {
public:
	Job() {}
	Job(const std::string& titleJob) {
	_titleJob = titleJob;
	}
	const std::string& GetTitleJob() const{
		return _titleJob;
	}

	friend std::ostream& operator <<(std::ostream& os, const Job& job);
	friend std::istream& operator >>(std::istream& is, Job& job);
	friend bool operator !=(const Job& one, const Job& other);

	~Job() {}
private:
	std::string _titleJob;
};

std::ostream& operator <<(std::ostream& os,const Job& job) {
	os << job._titleJob;
	return os;
}
std::istream& operator >>(std::istream& is, Job& job) {
	is >> job._titleJob;
	return is;
}
bool operator <(const Job& one, const Job& other) {
	return one.GetTitleJob() < other.GetTitleJob();
}
bool operator !=(const Job& one, const Job& other) {
	return one.GetTitleJob() != other.GetTitleJob();
}

//CheckList.txt
//Сотрудник
class Employee {
public:
	Employee() {}
	Employee(const std::string& name, const std::string& surname,
		const  std::string& patronymic, const double& rate) {
		_name = name;
		_surname = surname;
		_patronymic = patronymic;
		_rate = rate;
	}
	Employee(const std::string& name, const std::string& surname,
		const  std::string& patronymic, const double& rate, const Job& job) {
		_name = name;
		_surname = surname;
		_patronymic = patronymic;
		_rate = rate;
		_job = job;
	}
	const std::string& GetName() const {
		return _name;
	}
	const std::string& GetSurName() const {
		return _surname;
	}
	const std::string& GetPatronymic() const {
		return _patronymic;
	}
	const double& GetRate() const {
		return _rate;
	}
	const Job& GeJob() const {
		return _job;
	}
	Employee& operator =(const Job& newJob) {//
		_job = newJob;
	}
	
	friend std::ostream& operator <<(std::ostream& os, const Employee& job);
	friend std::istream& operator >>(std::istream& is, Employee& employee);
	~Employee() {}

	Job _job;
private:
	std::string _name, _surname, _patronymic;
	double _rate;
};
bool operator <(const Employee& employe1, const Employee& employe2) {
	return employe1.GetName()<employe2.GetName();
}
bool operator ==(const Employee& employe1, const Employee& employe2) {
	return employe1.GetSurName() == employe2.GetSurName();
}
std::ostream& operator <<(std::ostream& os, const Employee& employee) {
	os << employee.GetName() << " " << employee.GetSurName() << " " << employee.GetPatronymic() << " " << employee.GetRate() << " " << employee.GeJob();
	return os;
}
std::istream& operator >>(std::istream& is ,Employee& employee) {
	is >> employee._name >> employee._surname >> employee._patronymic >> employee._rate ;
	return is;
}

//Отдел
class Department {
public:
	Department() {}
	void ReadInfo(const char* link) {
		std::ifstream file(link);
		std::ofstream file1("werewer.txt");
		if (file.is_open())
			std::cout << "File is open!" << std::endl;
		else
			std::cout << "File don't open!" << std::endl;
		std::string Name, rate, name,surname, patronymic,jobb;
		bool endjobToMaxRate = true, endlistemployes=true;

		while (endjobToMaxRate && file >> Name >> rate) {
				for (auto c : rate) {
					if (isalpha(c)) {
						endjobToMaxRate = false;
						break;
					}
				}
				if (endjobToMaxRate == 0)
					break;
				_jobToMaxRate.emplace(Job{ Name }, std::stod(rate));
			}
		name = Name;
		surname = rate;
		file >> patronymic>>rate>>jobb;
		_emploees.insert(Employee{ name , surname, patronymic, std::stod(rate), Job{jobb} });
		while (endlistemployes && file >> name >> surname >> patronymic>> rate>> jobb) {
			for (auto c : name) {
				if (!isalpha(c)) {
					endlistemployes = false;
					break;
				}
			}
			if (endlistemployes == 0)
				break;
			_emploees.insert(Employee{ name , surname, patronymic, std::stod(rate), Job{jobb} });
			_jobToMaxRate[Job{ jobb }] -= std::stod(rate);
		}
		
		for (auto i : _jobToMaxRate) {
			file1 << i.first << " MaxRate " << i.second << std::endl;
		}
		for (auto i : _emploees) {
			file1 << i << std::endl;
		}
	}
	void Added_Employee(const Employee& people) { // добавление сотрудника в базу
		auto i = std::find(_emploees.begin(), _emploees.end(), people);
		if (i != _emploees.end()) {
			throw std::exception("Employee is already in the database.");
		}
		else
			_emploees.insert(people);
	}
	void GetAway_Employee(const Employee& people) {  //убираем сотрудника из базы
		auto i = std::find(_emploees.begin(), _emploees.end(), people);
		if (i == _emploees.end()) {
			throw std::exception("Employee get away in the database.");
		}
		else
			_emploees.erase(i);
	}
	void Hire_Employee(Employee& people,Job& job) {         // нанимаем человека на должность
		if ((_jobToMaxRate[job] -= people.GetRate()) >= 0)
		{
			_jobToMaxRate[job] -= people.GetRate();
		}
		else
			throw std::exception("Insufficient number of rate.");
	}
	void Remove_Employee(Employee& people) { // увольняем человека с должности
		if (people._job != Job{ "Non" }) {
			_jobToMaxRate[Job{ people.GeJob() }] += people.GetRate();
			people._job = Job{ "Non" };
		}
		else {
			throw std::exception("Employee don't work.");
		}
	}
	void Move_Employee(Employee& people, Job& job) {
		_jobToMaxRate[Job{ people.GeJob() }] += people.GetRate();
		if ((_jobToMaxRate[job] -= people.GetRate()) >= 0)
		{
			_jobToMaxRate[job] -= people.GetRate();
		}
		else
			throw std::exception("Insufficient number of rate.");
	}
	
	std::string Info() {
		std::string info;
		for (auto i : _jobToMaxRate) {
			info += i.first.GetTitleJob();
			info += ": ";
			info += i.second;
			info += '/n';
		}
		for (auto i : _emploees) {
			info += i.GetName();
			info +=" ";
			info += i.GetSurName();
			info += " ";
			info += i.GetPatronymic();
			info += " ";
			info += i.GetRate();
			info += " ";
			info += i.GeJob().GetTitleJob();
			info += '/n';
		}
		return info;
	}
	void WriteInfo(const char* link) {
		std::ofstream file2(link);
		for (auto i : _jobToMaxRate) {
			file2 << i.first << " MaxRate " << i.second << std::endl;
		}
		for (auto i : _emploees) {
			file2 << i << std::endl;
		}
	}
	~Department() {}
private:
	std::map <Job, double> _jobToMaxRate;
	std::set<Employee> _emploees;
};



int main() {
	Department a;
	a.ReadInfo("CheckList.txt");
	Employee people;
	while (1) {
		std::cout << "Enter employee you want add[1] /hire[2]/ remove[3] / dismiss[4] / move[5]/ info[6]." << std::endl;
		int choice;
		std::cin >> choice;
		bool exitEnter = false;
		switch (choice)
		{ 
		case 1: {
			std::cout << "Who do you want add?" << ": ";
			std::cin >> people;
			try {
				a.Added_Employee(people);
			}
			catch (const std::exception& e) {
				std::cerr << e.what() << std::endl;
			}
			continue;
		}
		case 2: {
			std::cout << "Who do you want hire" << ": ";
			std::cin >> people;
			std::cout << "Job?" << std::endl;
			Job job;
			std::cin >> job;
			try {
				a.Added_Employee(people);
			}
			catch (const std::exception& e) {
				std::cerr <<  e.what() << std::endl;
			}
			try {
				a.Hire_Employee(people,job);
			}
			catch (const std::exception& e) {
				std::cerr <<  e.what() << std::endl;
			}
			continue;
		}
		case 3: {
			std::cout << "Who do you want remove?" << ": ";
			std::cin >> people;
			try {
				a.Remove_Employee(people);
			}
			catch (const std::exception& e) {
				std::cerr <<  e.what() << std::endl;
			}
			continue;
		}
		case 4: {
			std::cout << "Who do you want dismiss?" << ": ";
			std::cin >> people;
			try {
				a.GetAway_Employee(people);
			}
			catch (const std::exception& e) {
				std::cerr <<  e.what() << std::endl;
			}
			
			continue;
		}
		case 5: {
			std::cout << "Who do you want move?" << ": ";
			std::cin >> people;
			Job job;
			std::cin >> job;
			try {
				a.Move_Employee(people,job);
			}
			catch (const std::exception& e) {
				std::cerr << e.what() << std::endl;
			}
			
			continue;
		}
		case 6: {
			std::cout<<a.Info();
			continue;
		}
		default: {
			exitEnter = true;
			break;
		}
		}
		if (exitEnter)
			break;
	}
	a.WriteInfo("Employees.txt");
}
