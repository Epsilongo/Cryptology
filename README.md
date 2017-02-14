
DES加密过程
======
* 64位密钥经子密钥产生算法产生出16个子密钥：K<sub>1</sub>，K<sub>2</sub>，···，K<sub>16</sub>，分别供第一次，第二次，······，第十六次加密迭代使用。<br>
* 64位明文首先经过初始置换IP(Initial Permutation)，将数据打乱重新排列并分成左右两半。左边32位构成L<sub>0</sub>，右边32位构成R<sub>0</sub><br>
* 由加密函数*f*实现子密钥K<sub>1</sub>对R<sub>0</sub>的加密，结果为32位的数据组*f*(R<sub>0</sub>,K<sub>1</sub>)。*f*(R<sub>0</sub>,K<sub>1</sub>)再与L<sub>0</sub>模2相加，又得到一个32位的数据组
