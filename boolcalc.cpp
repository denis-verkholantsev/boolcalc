#include<iostream>
#include"boolexpr.h"
#include<vector>
#include<fstream>

void print_help() {
	std::cout << "To calculate, get disjunction normal form, get conjunction normal form," << std::endl;
	std::cout << "get polinom of zhegalkin or check system of functions for copletness input the name of input-file with functions in separate strings," << std::endl;
	std::cout << "the name of output-file for writing result there and one of the keys:" << std::endl;
	std::cout << "-calc for calculating'\n-dnf for dnf\n-cnf for cnf\n-zh for polinim of zhegalkin" << std::endl;
	std::cout << "-isfull for checking for completness" << std::endl;
	std::cout << "To get truth table input the name of input-file with functions in separate strings," << std::endl;
	std::cout << "and key: -table";
}

int main(int argc, char* argv[]) {
	size_t action=0;
	if (argc > 3) {
		if (!strcmp(argv[1], "-calc")) {
			action = 1;
		}
		else if (!strcmp(argv[1], "-dnf")) {
			action = 2;
		}
		else if (!strcmp(argv[1], "-cnf")) {
			action = 3;
		}
		else if (!strcmp(argv[1], "-zh")) {
			action = 4;
		}
		else if (!strcmp(argv[1], "-isfull")) {
			action = 5;
		}
		else {
			std::cerr << "Unknown command";
			return -1;
		}
		std::ifstream Infile(argv[2], std::ios_base::binary);
		std::ofstream Outfile(argv[3], std::ios_base::binary);
		std::vector<BooleanExpression> func;
		while (Infile.peek() != EOF) {
			int ch;
			char buf[512];
			size_t i = 0;
			while ((ch = Infile.get()) != '\n'  && ch != EOF) {
				if (ch == '\r') {
					continue;
				}
				buf[i++] = ch;
			}
			buf[i] = '\0';
			BooleanExpression res(buf);
			func.push_back(res);
		}
		Infile.close();
		switch (action) {
		case 1:
			for (size_t i = 0; i < func.size(); ++i) {
				if (func[i].calc()) {
					Outfile << '1' << std::endl;
				}
				else {
					Outfile << '0' << std::endl;
				}
			}
			break;
		case 2:
			for (size_t i = 0; i < func.size(); ++i) {
				Outfile << func[i].dnf().operator std::string() << std::endl;
			}
			break;
		case 3:
			for (size_t i = 0; i < func.size(); ++i) {
				Outfile << func[i].cnf().operator std::string() << std::endl;
			}
			break;
		case 4:
			for (size_t i = 0; i < func.size(); ++i) {
				Outfile << func[i].zhegalkin().operator std::string() << std::endl;
			}
			break;
		case 5:
			if (isFullSystem(func)) {
				Outfile << "yes";
			}
			else {
				Outfile << "no";
			}
			break;
		}
		Outfile.close();
	}
	else if (argc == 3) {
		std::ifstream Infile(argv[2], std::ios_base::binary);
		while (Infile.peek() != EOF) {
			int ch;
			char buf[512];
			size_t i = 0;
			while ((ch = Infile.get()) != '\n' || ch != EOF) {
				buf[i++] = ch;
			}
			buf[i] = '\0';
			BooleanExpression res(buf);
			res.TruthTable();
			std::cout << std::endl;
		}
		Infile.close();
	}
	else if (argc == 2) {
		if (!strcmp(argv[1], "-h") || !strcmp(argv[1], "?")) {
			print_help();
	}
	else {
	std::cerr << "Uknown command";
		}
	}
}