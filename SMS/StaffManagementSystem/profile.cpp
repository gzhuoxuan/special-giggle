/*
职工信息管理系统

职工信息包括职工号、姓名、性别、年龄、学历、工资、住址、电话等（职工号不重复）。试设计一职工信息管理系统，使之能提供以下功能：
1、职工信息录入功能(职工信息用文件保存)－－输入
2、职工信息浏览功能－－输出
3、职工信息查询功能－－算法
查询方式： 按学历查询、按职工号查询、条件组合查询
职工信息删除、修改功能(可选项)
*/

/*
	26***Struct typedef
	48***get id
	65***input	
	100***save
	132***read from file
	164***delete
	226***search by Edu	
	251***search by ID
	326***main
*/

#include<stdio.h>
#include<string.h>

typedef struct
{ 
	int id; 
	char fname[20];
	char lname[20];
	char gender[3]; 
	int age; 
	char edu[50]; 
	char address[100]; 
	int wage; 
	char tele[20]; 
}Staff;

Staff Data_input(void);
void showAllStaff(void);
int saveStaff(Staff staff);
void searchByID(int id);
void searchByEdu(const char* edu);
void deleteID(int id);
int MenuList();


int Get_id()//AI
{
	FILE* fp = NULL;
	Staff temp;//temporarily store the info in temp
	int id = 1;

	if (fopen_s(&fp, "staff.dat", "rb") != 0 || fp == NULL)
		return 1;   // file not exist then create first record

	while (fread(&temp, sizeof(Staff), 1, fp) == 1)
		id++;

	fclose(fp);
	return id;
}


Staff Data_input() 
{ 
	Staff staff;
	staff.id = Get_id();
	int id;

	printf("\n\n\n\tNo.%d File", staff.id); 
	printf("\n\n\tFirst Name: "); 
	scanf_s("%s", staff.fname, (unsigned)sizeof(staff.fname)); 
	printf("\n\tLast Name: ");
	scanf_s("%s", staff.lname, (unsigned)sizeof(staff.lname));
	printf("\n\tGender(M/F): "); 
	scanf_s("%s", staff.gender, (unsigned)sizeof(staff.gender)); 
	printf("\n\tAge: "); 
	scanf_s("%d", &staff.age); 
	printf("\n\tEducation Background: "); 
	scanf_s("%s", staff.edu, (unsigned)sizeof(staff.edu)); 
	printf("\n\tAddress: "); 
	scanf_s("%s", staff.address, (unsigned)sizeof(staff.address)); 
	printf("\n\tWage: "); 
	scanf_s("%d", &staff.wage); 
	printf("\n\tTele: "); 
	scanf_s("%s", staff.tele, (unsigned)sizeof(staff.tele)); 

	int input1;
	printf("\n\n\t1. Save\n\n\t2. Back to Menu\n\n\t");
	scanf_s("%d", &input1);
	if (input1 == 1)//cannot use while bro
		saveStaff(staff);
	if (input1 == 2)
		MenuList();

	return staff;
}

int saveStaff(Staff staff)
{
	FILE* fp = NULL;//AI, file pointer
	errno_t err = fopen_s(&fp, "staff.dat", "ab");//append binary not wb cuz wb rewrite and clean the file
	int id;

	if (err != 0 || fp == NULL) //AI
	{
		printf("file open failed\n");
		return 0;
	}	
	
	fwrite(&staff, sizeof(Staff), 1, fp);//AI, write in
	fclose(fp);//AI close the file

	int i;
	printf("\tSuccessfully saved! Go back to the menu?\n\n\t1. Yes\t2. No\t3. Keep recording\n\n\t");
	scanf_s("%d", &i);
	if (i == 1)
	{
		MenuList();
		return 1;
	}
	else if (i == 2)
		return 0;
	else
	{
		Staff s = Data_input();
		return saveStaff(s);
	}
}

void showAllStaff(void)//AI
{
	FILE* fp = NULL;
	Staff staff;
	errno_t err = fopen_s(&fp, "staff.dat", "rb");

	if (err != 0 || fp == NULL)
	{
		printf("no file or open failed\n");
		return;
	}

	while (fread(&staff, sizeof(Staff), 1, fp) == 1)
	{
		printf("ID: %d\nFirst Name: %s\nLast Name: %s\nGender: %s\nAge: %d\nEducation: %s\nAddress: %s\nWage: %d\nTele: %s\n\n",
			staff.id, staff.fname, staff.lname, staff.gender,
			staff.age, staff.edu, staff.address, staff.wage, staff.tele);
	}

	fclose(fp);

	printf("\n\n\tKeep recording?\n\n\t1. Yes\t2. No\n\n\t");
	int temp;
	scanf_s("%d", &temp);
	if (temp == 1)
	{
		Staff s = Data_input();
	}
	else if (temp == 2)
		return;
}

void deleteID(int id)
{
	FILE* fp = NULL;
	FILE* temp = NULL;
	Staff s;
	int found = 0;

	searchByID(id);
	printf("\n\n\tSure to delete this profile?");
	int choice3;
	printf("\n\n\t1. Yes\t2. No (Go back to menu)\n\n\t");
	scanf_s("%d", &choice3);
	if (choice3 == 1)
	{
		fopen_s(&fp, "staff.dat", "rb");
		if (fp == NULL)
		{
			printf("file open failed\n");
			return;
		}

		fopen_s(&temp, "temp.dat", "wb");
		if (temp == NULL)
		{
			fclose(fp);
			printf("temp file open failed\n");
			return;
		}

		while (fread(&s, sizeof(Staff), 1, fp) == 1)
		{
			if (s.id == id)
			{
				found = 1;
				continue;
			}
			fwrite(&s, sizeof(Staff), 1, temp);
		}

		fclose(fp);
		fclose(temp);

		if (remove("staff.dat") != 0)
		{
			perror("remove failed");
			return;
		}

		else if (rename("temp.dat", "staff.dat") != 0)
		{
			perror("rename failed");
			return;
		}

		printf("\n\n\tDelete success\n");
	}
	else if (choice3 == 2)
		MenuList();
}


void searchByEdu(const char* edu)
{
	FILE* fp = NULL;
	Staff s;
	int found = 0;

	fopen_s(&fp, "staff.dat", "rb");
	if (!fp) return;

	while (fread(&s, sizeof(Staff), 1, fp) == 1)
	{
		if (strcmp(s.edu, edu) == 0)
		{
			printf("ID:%d  Front Name:%s  Last Name: %s  Edu:%s\n",
				s.id, s.fname, s.lname, s.edu);
			found = 1;
		}
	}

	if (!found)
		printf("no match\n");

	fclose(fp);
}

void searchByID(int id)
{
	FILE* fp = NULL;
	Staff s;
	errno_t err = fopen_s(&fp, "staff.dat", "rb");

	if (err != 0 || fp == NULL)
	{
		printf("file open failed\n");
		return;
	}

	while (fread(&s, sizeof(Staff), 1, fp) == 1)
	{
		if (s.id == id)
		{
			printf("FOUND\n");
			printf("ID:%d\nFront Name:%s\nLast Name:%s\nGender:%s\nAge:%d\nEdu:%s\nAddr:%s\nWage:%d\nTel:%s\n",
				s.id, s.fname, s.lname, s.gender, s.age,
				s.edu, s.address, s.wage, s.tele);
			fclose(fp);
			return;
		}
	}

	printf("not found\n");
	fclose(fp);
}

int MenuList() 
{
	int id;
	int choice;
	char edu[50];
	printf("\t\tStaff Information Management System\n\n");
	printf("\t1. Input information\n\n");
	printf("\t2. View Profile\n\n");
	printf("\t3. Search Profile\n\n");
	printf("\t4. Delete Profile\n\n\t");
	scanf_s("%d", &choice);
	
	if (choice == 1)
	{
		Data_input();
	}
	else if (choice == 2)
	{
		showAllStaff();
	}
	else if (choice == 3)	
	{
		printf("\n\t1. Search by ID\t\t2. Search by Education Background\n\n\t");
		int choice2;
		scanf_s("%d", &choice2);
		if (choice2 == 1)
		{
			printf("\n\n\tInput ID: ");
			scanf_s("%d", &id);
			searchByID(id);
		}
		else if (choice2 == 2)
		{
			printf("\n\n\tInput Education Background: ");
			scanf_s("%s", edu, (unsigned)sizeof(edu));
			searchByEdu(edu);
		}
	}
	else if (choice == 4)
	{
		printf("\n\n\tPlease input ID to delete: ");
		scanf_s("%d", &id);
		deleteID(id);
	}
	return 0;
}

int main(void) 
{ 
	MenuList();
	return 0;
} 