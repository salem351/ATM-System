#include <iostream>
#include <cstdlib>
#include <vector>
#include <fstream>
#include <iomanip>
#include <string>
using namespace std;

void Login();
void ShowATMMainMenueOption();
void ShowQuickWithdrawScreen();
void showNormalWithdrawScreen();
void showDepositScreen();

const string ClientFileName = "Client.txt";

enum enATMMainMenue {
	eQuickWithdraw = 1,
	eNormalWithdraw = 2,
	eDeposit = 3,
	eCheckBalance = 4,
	eExit = 5
};
struct stClient {
	string AccountNumber;
	string PinCode;
	string Name;
	string phone;
	double AccountBalance;
	bool DeletClient = false;
};

stClient CurrentClient;

vector <string> SpiltLineToVector(string line, string Delim) {
	vector <string> vString;
	string sWord = "";
	int pos = 0;

	while ((pos = line.find(Delim)) != std::string::npos) {

		sWord = line.substr(0, pos);
		if (sWord != " ") {
			vString.push_back(sWord);
		}
		line.erase(0, pos + Delim.length());
	}
	if (line != " ") {
		vString.push_back(line);
	}

	return vString;
}
stClient ConvertClientLineToRecord(string line) {
	stClient Client;

	vector <string> vString = SpiltLineToVector(line, "/##/");

	Client.AccountNumber = vString[0];
	Client.PinCode = vString[1];
	Client.Name = vString[2];
	Client.phone = vString[3];
	Client.AccountBalance = stod(vString[4]);

	return Client;
}
string ConvertCLientReocredToLine(stClient Client) {
	string LineClientDetails = "";

	LineClientDetails = Client.AccountNumber + "/##/";
	LineClientDetails += Client.PinCode + "/##/";
	LineClientDetails += Client.Name + "/##/";
	LineClientDetails += Client.phone + "/##/";
	LineClientDetails += to_string(Client.AccountBalance);

	return LineClientDetails;
}
vector <stClient> LoadClientFromFileToVector(string FileName) {
	vector <stClient> vClient;
	fstream Myfile;

	Myfile.open(FileName, ios::in);

	if (Myfile.is_open()) {
		stClient Client;
		string Line;

		while (getline(Myfile, Line)) {
			Client = ConvertClientLineToRecord(Line);
			vClient.push_back(Client);
		}

		Myfile.close();
	}
	return vClient;
}

vector <stClient> SaveClientsToFile(string FileName, vector <stClient> vClient) {

	fstream Myfile;

	Myfile.open(FileName, ios::out);

	string Line;

	if (Myfile.is_open()) {

		for (stClient& C : vClient) {

			if (C.DeletClient == false) {
				Line = ConvertCLientReocredToLine(C);
				Myfile << Line << endl;
			}
		}

		Myfile.close();
	}
	return vClient;
}

bool DepositAmountToAccountBalance(int Amount, string AccountNumber, vector <stClient> &vClient) {

	char DepositAmount = 'N';

	cout << "\n\nAre you sure you want to Perform this transactions? Y/N? ";
	cin >> DepositAmount;

	if (DepositAmount == 'Y' || DepositAmount == 'y') {

		for (stClient& C : vClient) {

			if (C.AccountNumber == AccountNumber) {
				C.AccountBalance += Amount;

				SaveClientsToFile(ClientFileName, vClient);

				cout << "\nDone Succssfully, New Balance is: " << C.AccountBalance << endl;
				return true;
			}

		}
	}
	return false;
}

short ReadATMMainMenueOption() {
	short num;

	cout << "Choose what do you want to do? [1 - 5]? ";
	cin >> num;

	return num;
}
void ReturnToATMMainMenue() {
	cout << "\nPress any key to go back to ATM main menue....";
	system("pause>0");
	ShowATMMainMenueOption();
}

void showCheckBalanceScreen() {
	cout << "==================================\n";
	cout << "\tCheck Balance Screen\n";
	cout << "==================================\n";
	cout << "Your Balance is: " << CurrentClient.AccountBalance << "\n\n";

}

int ReadQuickWithdrawOption() {
	int Choose;

	cout << "choose what to withdraw from [1] to [9]? ";
	cin >> Choose;

	return Choose;
}

short GetAmountBalance(short Option) {

	switch (Option) {

	case 1:
		return 20;
	case 2:
		return 50;
	case 3:
		return 100;
	case 4:
		return 200;
	case 5:
		return 400;
	case 6:
		return 600;
	case 7:
		return 800;
	case 8:
		return 1000;
	default:
		return 0;
	}
}
void PerformQuickWithdrawOption(short Option) {

	if (Option == 9) // Meaning Exit 
		return;

	short AmountBalance = GetAmountBalance(Option);

	while (AmountBalance > CurrentClient.AccountBalance) {
		cout << "\nThe amount exceeds you balance, make another balance.";
		cout << "\nPress any key to go back to Continue....";
		system("pause>0");
		ShowQuickWithdrawScreen();
	}

	vector <stClient> vClient = LoadClientFromFileToVector(ClientFileName);
	DepositAmountToAccountBalance(AmountBalance * -1, CurrentClient.AccountNumber, vClient);
	CurrentClient.AccountBalance -= AmountBalance;
}
void ShowQuickWithdrawScreen() {
	system("cls");
	cout << "==================================\n";
	cout << "\tQuick Withdraw\n";
	cout << "==================================\n";
	cout << "\t[1] 20" << setw(15) << left << "\t\t[2] 50 " << endl;
	cout << "\t[3] 100" << setw(15) << left << "\t\t[4] 200" << endl;
	cout << "\t[5] 400" << setw(15) << left << "\t\t[6] 600" << endl;
	cout << "\t[7] 800" << setw(15) << left << "\t\t[8] 1000" << endl;
	cout << "\t[9] Exit" << setw(15) << left << endl;
	cout << "==================================\n";
	cout << "Your Balance is: " << CurrentClient.AccountBalance << endl;

	PerformQuickWithdrawOption(ReadQuickWithdrawOption());
}

int ReadWithdrawAmount() {
	int AmountBalance;
	cout << "\nEnter an amount Multiple of 5's ?";
	cin >> AmountBalance;

	while (AmountBalance % 5 != 0) {
		cout << "\nEnter an amount Multiple of 5's ?";
		cin >> AmountBalance;
	}

	return AmountBalance;
}
void PerformNormalWithdrawOption() {

	int AmountBalance = ReadWithdrawAmount();

	while (AmountBalance > CurrentClient.AccountBalance) {
		cout << "\nThe amount exceeds you balance, make another balance.";
		cout << "\nPress any key to go back to Continue....";
		system("pause>0");
		showNormalWithdrawScreen();
	}

	vector <stClient> vClient = LoadClientFromFileToVector(ClientFileName);
	DepositAmountToAccountBalance(AmountBalance * -1, CurrentClient.AccountNumber, vClient);
	CurrentClient.AccountBalance -= AmountBalance;
}
void showNormalWithdrawScreen() {
	system("cls");
	cout << "==================================\n";
	cout << "\tNormal Withdraw\n";
	cout << "==================================\n";
	cout << "Your Balance is: " << CurrentClient.AccountBalance << endl;

	PerformNormalWithdrawOption();
	
}

int ReadDepositAmount() {

	int PositiveAmount = 0;

	do {
		cout << "\nEnter a psitive Deposit Amount? ";
		cin >> PositiveAmount;

	} while (PositiveAmount <= 0);

	return PositiveAmount;
}
void PerformDepositOption() {

	int DepoitAmount = ReadDepositAmount();

	vector <stClient> vClient = LoadClientFromFileToVector(ClientFileName);
	DepositAmountToAccountBalance(DepoitAmount, CurrentClient.AccountNumber, vClient);
	CurrentClient.AccountBalance += DepoitAmount;
}
void showDepositScreen() {
	system("cls");
	cout << "==================================\n";
	cout << "\tDeposit Screen\n";
	cout << "==================================\n";

	PerformDepositOption();
}

void PerformTheATMMainMenueOption(enATMMainMenue ATMMainMenue) {

	switch (ATMMainMenue) {

	case enATMMainMenue::eQuickWithdraw:
		system("cls");
		ShowQuickWithdrawScreen();
		ReturnToATMMainMenue();
		break;

	case enATMMainMenue::eNormalWithdraw:
		system("cls");
		showNormalWithdrawScreen();
		ReturnToATMMainMenue();
		break;

	case enATMMainMenue::eDeposit:
		system("cls");
		showDepositScreen();
		ReturnToATMMainMenue();
		break;

	case enATMMainMenue::eCheckBalance:
		system("cls");
		showCheckBalanceScreen();
		ReturnToATMMainMenue();
		break;

	case enATMMainMenue::eExit:
		system("cls");
		Login();
		break;
	}
}
void ShowATMMainMenueOption() {

	system("cls");
	cout << "==================================\n";
	cout << "\tATM Main Menue Screen\n";
	cout << "==================================\n";
	cout << "\t[1] QuickWithdarw\n";
	cout << "\t[2] NormalWithdarw\n";
	cout << "\t[3] Deposit\n";
	cout << "\t[4] Check balance\n";
	cout << "\t[5] LogOut\n";
	cout << "==================================\n";
	PerformTheATMMainMenueOption((enATMMainMenue)ReadATMMainMenueOption());
}

bool FindClientByAccountNumberandPinCode(string AccountNumber, string PinCode, stClient& Client) {

	vector <stClient> vClient = LoadClientFromFileToVector(ClientFileName);

	for (stClient& C : vClient) {

		if (C.AccountNumber == AccountNumber && C.PinCode == PinCode) {
			Client = C;
			return true;
		}
	}
	return false;
}
bool LoadClientsInfo(string AccountNumber, string PinCode) {

	if (FindClientByAccountNumberandPinCode(AccountNumber, PinCode, CurrentClient))
		return true;
	else
		return false;

}

void Login() {
	bool LoginFaild = false;

	string AccountNumber, PinCode;

	do {
		system("cls");
		cout << "____________________________________";
		cout << "\n\t Login Screen\n";
		cout << "____________________________________\n";

		if (LoginFaild) {
			cout << "Invaild AccountNumber/PinCode!\n";
		}

		cout << "Enter AccountNumber? ";
		cin >> AccountNumber;

		cout << "Enter PinCode? ";
		cin >> PinCode;

		LoginFaild = !LoadClientsInfo(AccountNumber, PinCode);

	} while (LoginFaild);

	ShowATMMainMenueOption();
}

int main() {

	Login();

	system("pause>0");
	return 0;

}