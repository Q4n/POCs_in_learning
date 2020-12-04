1. 修改init.bat中程序名称
2. 以管理员权限运行 init.bat 强制程序使用segment heap

注意: 这里面的poc, 大多数没有考虑后续堆布局和分配, 接着分配有可能导致分配失败的情况.

调试方法:
	直接用windbg调试即可

!heap -x addr
!heap -i addr -h heapbase

