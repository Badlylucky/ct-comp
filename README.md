# ct-competition

https://fmselab.github.io/ct-competition/

- 生成時間とサイズでの比較
- ベンチマークは提供される
- ソースコードの提出は必要なし

- no constraintsに提出
 - With only boolean parameters
 - MCA
 - Uniform with n > 2

# 制約
パラメータは全て0以上の整数で表す。
## UNIFORM 1
```
len(v) == 2 (1 or 0)
2<= k <= 20
```
25 case
## UNIFORM 2
```
2 <= len(v) <= 20
2 <= k <= 20
```
25 case

vの大きさは全てのパラメータで等しい(v[0]==v[1]==...==v[k-1])
## MCA
```
2 <= k <= 20
2 <= len(v[i]) <= 20 (0 <= i <= k-1)
```
50 case

## 入力
```
T K
V[0] V[1] V[2] ... V[K-1]
```

## 出力
```
N
A[0]
A[1]
...
A[N-1]
```

ただし、A[i]については以下に従う
```
v[0] v[1] ... v[K-1]
```