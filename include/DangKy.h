#include <string>
using namespace std;

struct DangKy
{
    string maSV;       
    float diem;        
    bool huyDangKy;   
};

struct NodeDangKy
{
    DangKy data;
    NodeDangKy* next;
};

struct DanhSachDangKy
{
    NodeDangKy* head;
    int soLuong;
};
