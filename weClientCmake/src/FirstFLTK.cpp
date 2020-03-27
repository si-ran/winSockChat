#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_PNG_Image.H>
#include <string>
#include <iostream>
#include <sstream>

using std::cout;
using std::endl;
using std::string;
using std::stringstream;

//string转换函数
template<typename T> string toString(const T& t) {
    stringstream ss{};     //创建一个格式化输出流
    ss << t;               //把值传递如流中
    return ss.str();
}

int i = 1000;
Fl_Box* box;
Fl_PNG_Image* rgb;

//用于刷新
void recvFunc1( void* ) {
    string file_name = string("E:/ps/video/src-") + toString<int>(i) + ".png";
    const char* file = file_name.c_str();
    rgb = new Fl_PNG_Image(file);
    box->image(rgb);
    box->redraw();
    cout << file << endl;
    i++;
    Fl::repeat_timeout(0.01, recvFunc1);
}

int main(int argc, char** argv) {
    Fl_Window* window = new Fl_Window(640, 560);
    box = new Fl_Box(20, 20, 500, 500);
    window->end();
    window->show(argc, argv);
    Fl::add_timeout(0.05, recvFunc1);
    return Fl::run();
}