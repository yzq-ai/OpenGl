## Cherno ϵ�н̴̳�����Ŀ�ֿ�

### ��ǰ����

* ����

  ```
  �汾	Windows 10 רҵ��
  �汾��	20H2
  ��װ����	2021-05-12
  ����ϵͳ�ڲ��汾	19042.1415
  ����	Windows Feature Experience Pack 120.2212.3920.0
  
  ```

* IDE 

  ```
  Microsoft Visual Studio Community 2022 (64 λ) �汾 17.0.4
  ```

### P2. ��VS�д�����Ŀ, ����GLFW��OpenGL

* ���� [GLFW](https://www.glfw.org/)

  [����ת����](https://github.com/glfw/glfw/releases/download/3.3.6/glfw-3.3.6.zip)

  ```c
  /**
   * ���ø��Ӱ���Ŀ¼
   * ��Ŀ => OpenGL���� => C\C++ => ���� => ���Ӱ���Ŀ¼
   * ���: $(SolutionDir)Dependencies\GLFW\include
   * ���ø��ӿ�Ŀ¼
   * ��Ŀ => OpenGL���� => ������ => ���� => ���ӿ�Ŀ¼
   * ���: $(SolutionDir)Dependencies\GLFW\lib-vc2022
   * ���ø���������
   * ��Ŀ => OpenGL���� => ������ => ���� => ����������
   * ���: glfw3.lib
   ** /
  ```
  
* �������

  * `Application.obj : error LNK2019: �޷��������ⲿ���� __imp__glClear@4������ _main �������˸÷���`

    ```c
    /**
     * ���丽��������(opengl32.lib)
     * ��Ŀ => OpenGL���� => ������ => ���� => ����������
     * ���: opengl32.lib
     **/
    ```
  
  * `glfw3.lib(win32_init.obj) : error LNK2019: �޷��������ⲿ���� __imp__RegisterDeviceNotificationW@12������ __glfwPlatformInit �������˸÷���`
  
    ```c
    /**
     * ���丽��������(user32.lib)
     * ��Ŀ => OpenGL���� => ������ => ���� => ����������
     * ���: user32.lib
     **/
  	```

  * `glfw3.lib(win32_monitor.obj) : error LNK2019: �޷��������ⲿ���� __imp__CreateDCW@16������ __glfwPlatformGetGammaRamp �������˸÷���`

    ```c
    /**
     * ���丽��������(gdi32.lib)
     * ��Ŀ => OpenGL���� => ������ => ���� => ����������
     * ���: gdi32.lib
     **/
    ```

  * `glfw3.lib(win32_window.obj) : error LNK2019: �޷��������ⲿ���� __imp__DragQueryFileW@16������ _windowProc@16 �������˸÷���`

    ```c
    /**
     * ���丽��������(shell32.lib)
     * ��Ŀ => OpenGL���� => ������ => ���� => ����������
     * ���: shell32.lib
     **/
    ```

  * `LINK : warning LNK4098: Ĭ�Ͽ⡰MSVCRT�����������ʹ�ó�ͻ����ʹ�� /NODEFAULTLIB:library`

    ```c
    /**
     * �����ض�Ĭ�Ͽ�(msvcrt.lib)
     * ��Ŀ => OpenGL���� => ������ => ���� => �����ض�Ĭ�Ͽ�
     * ���: msvcrt.lib
     **/
    ```

* ��ǰ���������� `glfw3.lib;opengl32.lib;user32.lib;gdi32.lib;shell32.lib`