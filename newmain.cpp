#include <iostream>
#include <string.h>
#include <sstream>
#include <fstream>
#include <stdlib.h>
#include <cstdio>

using namespace std;

struct userinfo{
                char name[50];
                char address[50];
                char contact_no[50];
                char pancard_no[50];
                int login_id;
                char password[50];
                int login_status;
                };

class Userfile{

    struct userinfo userfile;

    public:
            void save(const char *fname, char *sname, char *sno){

                fstream saved;
                fstream workfile;
                workfile.open(fname, ios::app);
                saved.open("directory.dat", ios::in);

                while(saved.read((char *)&userfile, sizeof(userinfo)))
                    {
                        if(!strcmp(sname, userfile.name) && !strcmp(sno, userfile.contact_no))
                            workfile.write((char *)&userfile, sizeof(userfile));
                    }
                workfile.close();
                saved.close();
            }

            void display(const char *fname){

                fstream readsaved;

                readsaved.open(fname, ios::in);

                while(readsaved.read((char *)&userfile, sizeof(userinfo)))
                    {
                        cout<<userfile.name<<"\t"<<userfile.contact_no<<"\t"<<userfile.address<<endl;
                    }
                readsaved.close();
            }

            void delcontact(const char *fname){

                fstream delsaved;
                fstream temp;
                char delname[50];
                char delno[50];

                cout<<"Enter name and number of contact to be deleted:\n";
                cin>>delname;
                cin>>delno;

                delsaved.open(fname, ios::in);
                temp.open("temp.dat", ios::out);

                while(delsaved.read((char *)&userfile, sizeof(userinfo)))
                {
                        if(!strcmp(delname, userfile.name) && !strcmp(delno, userfile.contact_no))
                            continue;
                        else
                            temp.write((char *)&userfile, sizeof(userfile));
                }
                delsaved.close();
                temp.close();

                remove(fname);
                rename("temp.dat", fname);

            }
    };

class User{

    public:


        struct userinfo user;

        void registration(){

                cout<<"---------------------------------"<<endl;
                cout<<"\t\tWelcome to Registration\t\t"<<endl;
                cout<<"---------------------------------"<<endl;

                char blank[10];
                cin.getline(blank, sizeof(blank));

                fstream registerfile;
                registerfile.open("adminbuffer.dat", ios::app|ios::binary);

                cout<<"Enter your name:";
                cin.getline(user.name, sizeof(user.name));

                cout<<"Enter contact number:"<<endl;
                cin.getline(user.contact_no, sizeof(user.contact_no));

                cout<<"Enter address:"<<endl;
                cin.getline(user.address, sizeof(user.address));

                cout<<"Enter PAN card number:"<<endl;
                cin.getline(user.pancard_no, sizeof(user.pancard_no));

                user.login_status=0;

                ifstream filelogin;
                filelogin.open("registration.txt", ios::in|ios::app ); //opening in read mode

                string line; //Stores first line of the file
                string login_id;
                string ps;

                getline(filelogin, line); //Reading first line
                stringstream iss(line);
                getline(iss, login_id, '|');//Parsing for login index, before |
                getline(iss, ps, '|' );
                stringstream convert(login_id);
                convert>>user.login_id; //converting login index string to int

                stringstream conv(ps);
                conv>>user.password; //Holds new user password
                filelogin.close();

                registerfile.write((char *)&user, sizeof(struct userinfo));
                registerfile.close();
                user.login_id=user.login_id+1;

                ofstream loginupdate;
                loginupdate.open("registration.txt", ios::out); //Opening in write mode
                loginupdate << user.login_id <<"|"<<"iam"<<user.login_id<<"|"; //Stores login index on first line of file
                loginupdate.close();

                cout<<"Add request sent to admin.\n";
                cout<<user.name<<", your credentials are:\nLogin ID:"<<user.login_id<<"\nPassword:"<<user.password<<"\n";

        }

        int login(){

            int lid;
            bool ok=false;
            int check=0;
            int flag=0;
            char pswd[50];

            while(!ok){

                    cout<<"Enter login id:"<<endl;
                    cin>>lid;
                    if(!std::cin.fail() && (std::cin.peek()==EOF || std::cin.peek()=='\n'))
                    {
                        ok = true;
                    }
                    else
                        {
                        std::cin.clear();
                        std::cin.ignore(256,'\n');
                        std::cout << "Error, Enter a valid ID." << std::endl;
                        }
            }
            cout<<"Password:"<<endl;
            cin>>pswd;

            fstream userlogin;
            fstream uniquelogin;

            userlogin.open("directory.dat", ios::in);
            uniquelogin.open("tempdirectory.dat", ios::out);

            struct userinfo temp;

            while(userlogin.read((char *)&temp, sizeof(userinfo)))
            {
                if(!strcmp(pswd, temp.password)&& lid==temp.login_id)
                    {
                    if(temp.login_status==1)
                    {
                        cout<<"User already logged in.\n";
                    }
                    else
                    {
                        temp.login_status=1;
                    }
                    flag=1;
                    user=temp;
                    }

                else if(lid!=temp.login_id && temp.login_status==1)
                    {
                        check=1;
                        user=temp;
                    }

                uniquelogin.write((char *)&temp, sizeof(userinfo));
            }

            uniquelogin.close();
            userlogin.close();
            remove("directory.dat");
            rename("tempdirectory.dat", "directory.dat");

            if(check==1)
                {
                cout<<"Another user is logged in. Try later!\n";
                return 0;
                }
            else if(check==0 && flag==1)
                return 1;
            else
                return 0;

        }

       void logout(){

            fstream out;
            fstream updateout;
            struct userinfo temp;

            updateout.open("tempdirectory.dat", ios::out);
            out.open("directory.dat", ios::in);

            cout<<"\n\tUSER LOGIN STATUS\n";

            while(out.read((char *)&temp, sizeof(struct userinfo))){

                if(getlogin()==temp.login_id && !strcmp(temp.password, getpass())&&temp.login_status==1)
                    {
                        temp.login_status=0;
                        user=temp;
                    }
                updateout.write((char *)&temp, sizeof(struct userinfo));
                cout<<"\tUser:"<<temp.login_id<<"\tStatus:"<<temp.login_status<<endl;
            }

            out.close();
            updateout.close();

            remove("directory.dat");
            rename("tempdirectory.dat", "directory.dat");

        }

        int getlogin(){

            if(user.login_status==0)
                user.login_id=0;

            return user.login_id;
            }

        char * getpass(){

            return user.password;

            }

        void modify(){

            int modifychoice=0;

            fstream modifyfile;
            fstream tempfile;

            struct userinfo tomodify;

            tempfile.open("tempdirectory.dat",ios::out);
            modifyfile.open("directory.dat", ios::in|ios::out);

            while(modifyfile.read((char *)&tomodify, sizeof(userinfo)))
            {
                if(getlogin()==tomodify.login_id && !strcmp(tomodify.password, getpass()))
                    {
                        cout<<"\nModify by: 1. Contact Number 2. Address\n";
                        cin>>modifychoice;

                        if(modifychoice==1)
                            {
                            cout<<"Enter new number:\n";
                            cin>>tomodify.contact_no;
                            }
                        else if(modifychoice==2)
                        {
                            cout<<"Enter new address:\n";
                            cin>>tomodify.address;
                        }
                    }

                tempfile.write((char *)&tomodify, sizeof( userinfo));
            }
            tempfile.close();
            modifyfile.close();

            remove("directory.dat");
            rename("tempdirectory.dat","directory.dat");
        }

       void Delete(){

            fstream deletefile;
            fstream tempfile;

            struct userinfo todelete;

            tempfile.open("tempdirectory.dat",ios::out);
            deletefile.open("directory.dat", ios::in|ios::out);

            while(deletefile.read((char *)&todelete, sizeof(userinfo)))
            {
                if(getlogin()==todelete.login_id && !strcmp(todelete.password, getpass()))
                    {
                        continue;
                    }

                tempfile.write((char *)&todelete, sizeof(userinfo));
            }
            user.login_id=0;
            tempfile.close();
            deletefile.close();

            remove("directory.dat");
            rename("tempdirectory.dat","directory.dat");

        }

        void Search(){

            int searchopt;
            char searchname[50];
            char searchno[50];
            char searchaddr[50];

            cout<<"Your options are, search by:\n1. Name\n2. Contact Number\n3. Address\n";
            cin>>searchopt;

            if(searchopt==1)
            {
                cout<<"Enter name to be searched:\n";
                cin>>searchname;
            }
            else if(searchopt==2)
            {
                cout<<"Enter number to be searched for:\n";
                cin>>searchno;
            }
            else if(searchopt==3)
            {
                cout<<"Enter address to be searched from:\n";
                cin>>searchaddr;
            }
            else
                cout<<"Invalid choice.\n";

            fstream searchfile;
            searchfile.open("directory.dat", ios::in);

            struct userinfo tosearch;

            while(searchfile.read((char *)&tosearch, sizeof(struct userinfo)))
            {
                if(searchopt==1 && !strcmp(tosearch.name, searchname))
                        cout<<tosearch.name<<"\t"<<tosearch.contact_no<<"\t"<<tosearch.address<<endl;
                else if(searchopt==2 && !strcmp(tosearch.contact_no, searchno))
                        cout<<tosearch.name<<"\t"<<tosearch.contact_no<<"\t"<<tosearch.address<<endl;
                else if(searchopt==3 && !strcmp(tosearch.address, searchaddr))
                        cout<<tosearch.name<<"\t"<<tosearch.contact_no<<"\t"<<tosearch.address<<endl;
            }
        }

        void emergencyservices(){

            fstream emerg;
            emerg.open("emergency.dat", ios::in);
            struct userinfo temp;

            cout<<"\tName\tNumber\tAddress\n";
            while(emerg.read((char *)&temp, sizeof(userinfo))){
                cout<<"\t"<<temp.name<<"\t"<<temp.contact_no<<"\t"<<temp.address<<endl;
            }

            emerg.close();
        }
};

class Admin{

    char admin_id[50];
    char admin_password[50];

    public:
        int login_status;
        void add_record(){

            int adminchoice;
            int choice;

            struct userinfo addrec;
            fstream adminfile;
            fstream admintodirec;
            fstream tempadminbuffer;

            cout<<"Your choices are:\n1. Accept or Reject User Record.\n2. Add Emergency Service\nEnter choice:";
            cin>>choice;

            if(choice==1)
            {

                cout<<"Requests:"<<endl;

                adminfile.open("adminbuffer.dat", ios::in|ios::out);
                tempadminbuffer.open("tempadminbuffer.dat", ios::out);
                cout<<"\tLogin ID\tName\tNumber\tAddress\n";

                while(adminfile.read((char *)&addrec, sizeof(userinfo)))
                {
                     cout<<"\t"<<addrec.login_id<<"\t"<<addrec.name<<"\t"<<addrec.contact_no<<"\t"<<addrec.address<<endl;
                     cout<<"\n1. Accept add request\n0. Skip request\n";
                     cin>>adminchoice;

                     if(adminchoice==1)
                        {   admintodirec.open("directory.dat", ios::app);
                            admintodirec.write((char *)&addrec, sizeof(userinfo));
                            admintodirec.close();
                        }
                    else
                    {
                        tempadminbuffer.write((char *)&addrec, sizeof(userinfo));
                        cout<<"Thank you.\n";
                    }
                }
                adminfile.close();
                tempadminbuffer.close();
                remove("adminbuffer.dat");
                rename("tempadminbuffer.dat", "adminbuffer.dat");
            }

            else if(choice==2){

                fstream addemerg;
                addemerg.open("emergency.dat", ios::app);
                struct userinfo temp;

                cout<<"Enter name of emergency service:";
                cin>>temp.name;
                cout<<"Enter contact number:";
                cin>>temp.contact_no;
                cout<<"Enter address:";
                cin>>temp.address;

                addemerg.write((char *)&temp, sizeof(userinfo));
                addemerg.close();
            }
        }

        int login(){

            cout<<"Enter admin id:"<<endl;
            cin>>admin_id;
            cout<<"Enter admin password:"<<endl;
            cin>>admin_password;

            if(!strcmp(admin_id,"admin")&&!strcmp(admin_password,"admin"))
                {
                    login_status=1;
                    cout<<"Login successful"<<endl;
                }
            else
                {   login_status=0;
                    cout<<"Incorrect credentials"<<endl;
                }
            return login_status;
            }
};

class Directory{

    public:

        void display(){

            fstream direc;
            direc.open("Directory.dat", ios::in);
            struct userinfo directuse;

            cout<<"\tName\tNumber\tAddress\n";

            while(direc.read((char *)&directuse, sizeof(struct userinfo)))
            {
                cout<<"\t"<<directuse.name<<"\t"<<directuse.contact_no<<"\t"<<directuse.address<<"\n";
            }
            direc.close();

            }
};
int main(){

    int user_choice=0;

    Admin ad;
    User U;
    Directory D;
    Userfile uf;

    const char *s;
    string st;

    cout<<"------------------------------------------------------------------------------------------\n";
    cout<<"\tInstructions:"<<endl;
    cout<<"------------------------------------------------------------------------------------------\n";
    cout<<"\t- Each user must register to add themselves to the telephone directory.\n";
    cout<<"\t- After registration, the admin decides whether to add or reject the user's request.\n";
    cout<<"\t- After logging in, the user can modify or delete their records in the directory.\n";
    cout<<"\t- Services of saving contacts and emergency are also provided.\n";

    do{

        cout<<"------------------------------------------------------------------------------------------\n";
        cout<<"\t\tTELEPHONE DIRECTORY SYSTEM\n";
        cout<<"------------------------------------------------------------------------------------------\n";
        cout<<"\tCURRENT USER:"<<U.getlogin()<<endl;
        cout<<"------------------------------------------------------------------------------------------\n\n";
        cout<<"\tThe Telephone Directory System offers the following features:\n"<<endl;
        cout<<"\t1. User Registration "<<endl;
        cout<<"\t2. Login "<<endl;
        cout<<"\t3. Display Directory"<<endl;
        cout<<"\t4. Modify record"<<endl;
        cout<<"\t5. Delete record"<<endl;
        cout<<"\t6. Search directory"<<endl;
        cout<<"\t7. Saved Contacts Services"<<endl;
        cout<<"\t8. Emergency Services"<<endl;
        cout<<"\t9. Logout"<<endl;
        cout<<"\t10. Exit\n\n\tEnter choice:";
        cin>>user_choice;
        stringstream ss;

        switch(user_choice){

            case 1:
                User user_reg;
                user_reg.registration(); //completed
                break;

            case 2:
                int login_type;
                cout<<"Enter login type:\n1. Admin\n2. User"<<endl;
                cin>>login_type;
                if(login_type==1&&ad.login_status!=1)
                        {
                        if(ad.login()==1)
                            {
                                ad.add_record();
                                ad.login_status=1;
                            }
                        else
                            cout<<"Incorrect admin credentials\n";
                        }

                else if(login_type==1&&ad.login_status==1)
                        ad.add_record();

                else if(login_type==2)
                    {
                        if(U.login()==1)
                            cout<<"Login successful\n";
                        else
                            cout<<"Incorrect credentials\n";
                    }

                break;

            case 3:
                D.display();
                break;

            case 4:
                U.modify();
                break;

            case 5:
                U.Delete();
                break;

            case 6:
                U.Search();
                break;

            case 7:
                D.display();
                char savename[50];
                char saveno[50];
                int savechoice;

                ss<<U.getlogin();
                st = ss.str() + ".dat";
                s = st.c_str();

                cout<<"1. Save Contact\n2. Display Saved Contacts\n3. Remove Contact\n";
                cin>>savechoice;
                if(savechoice==1)
                {
                    cout<<"Enter name and number of contact to be saved:\n";
                    cin>>savename;
                    cin>>saveno;
                    uf.save(s,savename, saveno);
                }
                else if(savechoice==2){
                    uf.display(s);
                }
                else if(savechoice==3){
                    uf.delcontact(s);
                }

                break;

            case 8:
                U.emergencyservices();
                break;

            case 9:
                U.logout(); //completed
                break;

            case 10:
                exit(0);
                }
    } while(user_choice <= 10);

   return 0;
}
