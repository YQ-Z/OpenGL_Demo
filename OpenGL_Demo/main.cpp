//移入GLTool的着色器管理器
#include "GLShaderManager.h"

//GLTool.h头⽂件包含了⼤部分GLTool中类似C语⾔的独⽴函数
#include "GLTools.h"

/*
 在Mac 系统下，`#include<glut/glut.h>`
 在Windows 和 Linux上，我们使⽤freeglut的静态库版本并且需要添加⼀个宏
 */
#include <glut/glut.h>

//定义一个批次容器，是GLTools的容器类
GLBatch triangleBatch;
//定义一个着色管理器
GLShaderManager shaderManager;

/*
 窗口大小改变时接受新的宽度和高度，其中0,0代表窗口中视口的左下角坐标，w，h代表像素
 changeSize 触发条件:
    1. 新建窗⼝
    2. 窗⼝尺⼨发⽣调整
 处理业务:
    1. 设置OpenGL 视⼝
    2. 设置OpenGL 投影⽅式等.
 */
void ChangeSize(int w,int h) {
    glViewport(0,0, w, h);
}

/*
 为程序作一次性的设置，设置渲染图形的相关顶点数据/颜色数据等数据装备工作
 setupRC 触发条件:
    1.⼿动main函数触发
 处理业务:
    1.设置窗⼝背景颜⾊
    2.初始化存储着⾊器shaderManager
    3.设置图形顶点数据
    4.利⽤GLBatch 三⻆形批次类,将数据传递到着⾊器
*/
void SetupRC() {
    //设置背影颜色
    glClearColor(0.0f,0.0f,1.0f,1.0f);
    
    //初始化着色管理器
    shaderManager.InitializeStockShaders();
    
    //设置三角形，其中数组vVert包含所有3个顶点的x,y,笛卡尔坐标对。
    GLfloat vVerts[] = {
        -0.5f,0.0f,0.0f,
        0.5f,0.0f,0.0f,
        0.0f,0.5f,0.0f,
    };
    
    //批次处理
    triangleBatch.Begin(GL_TRIANGLES,3);
    triangleBatch.CopyVertexData3f(vVerts);
    triangleBatch.End();
}

/*
 开始渲染：当屏幕发⽣变化/或者开发者主动渲染会调⽤此函数,⽤来实现数据->渲染过程
 RenderScene 触发条件:
    1.系统⾃动触发
    2.开发者⼿动调⽤函数触发.
 处理业务:
    1.清理缓存区(颜⾊,深度,模板缓存区等)
    2.使⽤存储着⾊器
    3.绘制图形.
*/
void RenderScene(void) {
    
    //清除一个或一组特定的缓冲区
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);
    
    //设置一组浮点数来表示红色
    GLfloat vRed[] = {1.0f,0.0f,0.0f,1.0f};
    
    //传递到存储着色器，即GLT_SHADER_IDENTITY着色器，这个着色器只是使用指定颜色以默认笛卡尔坐标系在屏幕上渲染几何图形
    shaderManager.UseStockShader(GLT_SHADER_IDENTITY,vRed);
    
    //提交着色器
    triangleBatch.Draw();
    
    //将在后台缓冲区进行渲染，然后在结束时交换到前台
    glutSwapBuffers();
}

int main(int argc,char* argv[]) {
    
    //设置当前工作目录，针对MAC OS X(对Windows中是没必要的。因为windows下默认的工作目录就是与程序可执行目录相同)
    gltSetWorkingDirectory(argv[0]);
    
    //初始化GLUT库
    glutInit(&argc, argv);
    
    /*初始化双缓冲窗口，其中标志GLUT_DOUBLE、GLUT_RGBA、GLUT_DEPTH、GLUT_STENCIL分别指
     双缓冲窗口、RGBA颜色模式、深度测试、模板缓冲区*/
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA|GLUT_DEPTH|GLUT_STENCIL);
    
    //设置GLUT窗口大小
    glutInitWindowSize(800,600);
    //设置窗口标题
    glutCreateWindow("Triangle");
    
    //注册回调函数
    glutReshapeFunc(ChangeSize);
    glutDisplayFunc(RenderScene);
    
    //驱动程序的初始化中没有出现任何问题。
    /*
     如果要使用glew相关的函数，那么一定要先对glew初始化。
     如果不初始化，那么虽然编译相关的程序不会报错，但是在运行时，则会报告错误
     GLEW是一个跨平台的C++扩展库，基于OpenGL图形接口.GLEW能自动识别你的平台所支持的全部OpenGL高级扩展涵数。也就是说，只要包含一个glew.h头文件，你就能使用gl,glu,glext,wgl,glx的全部函数
     */
    GLenum err = glewInit();
    if(GLEW_OK != err) {
        fprintf(stderr,"glew error:%s\n",glewGetErrorString(err));
        return 1;
    }
    
    //调用SetupRC
    SetupRC();
    glutMainLoop();
    return 0;
    
}

