struct Student{
        char name[256];
        char rollno[50];
        int courses[6];
        char username[50];
        char password[50];
        // bool isActivated;
        // bool isBlocked;
};

struct Faculty{
        int faculty_id;
        char name[256];
        int courses[10];
        char username[50];
        char password[50];
};


struct faculty_count
    {
        int count;
    };