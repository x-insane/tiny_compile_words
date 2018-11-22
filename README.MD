## 编译原理实验一： TINY+ 词法分析器

### 编译代码

```
# windows
g++ words_analysis.cpp -std=c++11 -o words_analysis.exe

#linux
g++ words_analysis.cpp -std=c++11 -o words_analysis
```

### 运行测试

```
# windows
words_analysis.exe --test

#linux
./words_analysis --test
```

### 测试样例

#### test/test1.txt

```
true	false	or	and	not
int	bool	string	while	do
if	then	else	end	repeat
until	read	write	,	;
:=	+	-	*	/
(	)	<	=	>
<=	>=	a2c	123	'EFG'

{The following symbols are incorrect}
$ &
123man
miss left single quotation mark'
'miss right single quotation mark
miss left curly bracket}
{miss right curly bracket
```

#### test/test2.txt

```
{this is an example}
int A,B;
bool C;
string D;
D:= 'scanner';
C:=A and not B;
while A<=D do 
	A:=A*2
end;
if(A>-B)then C:=true;end;
if(A<=-B)then C:=false;end;
```

#### test/output.txt

```
---- START TEST test/test1.txt ----
ERROR IN LINE 10:1 unknown symbol: $
ERROR IN LINE 10:3 unknown symbol: &
ERROR IN LINE 11:4 here must be a digit: m
ERROR IN LINE 11:5 here must be a digit: a
ERROR IN LINE 11:6 here must be a digit: n
ERROR IN LINE 12:32 here must be a letter: '
ERROR IN LINE 13:34 unclosed string
ERROR IN LINE 14:24 here must be a letter: }
ERROR IN LINE 15:26 unclosed annotation

test failed.
------ END TEST test/test1.txt ----

---- START TEST test/test2.txt ----
(KEY_INT, int)
(ID, A)
(OP_COMMA, ,)
(ID, B)
(OP_SEMICOLON, ;)
(KEY_BOOL, bool)
(ID, C)
(OP_SEMICOLON, ;)
(KEY_STRING, string)
(ID, D)
(OP_SEMICOLON, ;)
(ID, D)
(OP_ASSIGN, :=)
(STRING, 'scanner')
(OP_SEMICOLON, ;)
(ID, C)
(OP_ASSIGN, :=)
(ID, A)
(KEY_AND, and)
(KEY_NOT, not)
(ID, B)
(OP_SEMICOLON, ;)
(KEY_WHILE, while)
(ID, A)
(OP_LEQ, <=)
(ID, D)
(KEY_DO, do)
(ID, A)
(OP_ASSIGN, :=)
(ID, A)
(OP_MUL, *)
(NUMBER, 2)
(KEY_END, end)
(OP_SEMICOLON, ;)
(KEY_IF, if)
(OP_LP, ()
(ID, A)
(OP_GTR, >)
(OP_SUB, -)
(ID, B)
(OP_RP, ))
(KEY_THEN, then)
(ID, C)
(OP_ASSIGN, :=)
(KEY_TRUE, true)
(OP_SEMICOLON, ;)
(KEY_END, end)
(OP_SEMICOLON, ;)
(KEY_IF, if)
(OP_LP, ()
(ID, A)
(OP_LEQ, <=)
(OP_SUB, -)
(ID, B)
(OP_RP, ))
(KEY_THEN, then)
(ID, C)
(OP_ASSIGN, :=)
(KEY_FALSE, false)
(OP_SEMICOLON, ;)
(KEY_END, end)
(OP_SEMICOLON, ;)

test passed.
------ END TEST test/test2.txt ----

1 test(s) passed. 1 test(s) failed.
```
