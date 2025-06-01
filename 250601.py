import pandas as pd

# # Series 선언 방법
# # 1. index 와 name 모두 입력 시
# a = pd.Series(['Data Mining', 1, 2, 3, 4],
#               index=['a','b','c','d','e'],
#               name='example')

# # 2. index 와 name 모두 입력하지 않았을 시
# b = pd.Series(['Data Mining', 1, 2, 3, 4])

# print(a,b,sep='\n\n')

# Series 생성 : list-based
# a = pd.Series(['bae','lee','kim'])

# # list-based Series
# print("Series.index\n",a.index,end="\n")
# print("Series.values\n",a.values,end="\n")

# # Series 생성 : Dict-based
# b = pd.Series({'Kim':23, 'lee':21, 'bae':25})

# # list-based Series
# print("Series.index\n",b.index,end="\n")
# print("Series.values\n",b.values,end="\n")

# # Series 열이름(name)/행이름(index) 추가하기
# a = pd.Series(['lee','kim','bae'],index=[1,2,3])
# a.name ='columns name'
# a.index.name = 'rows name'
# print(a)

# a = pd.Series(['lee','kim','bae'],index=['a','b','c'])
# print(a,"\n",a['a'],'\t',a['c']) # 인덱스 value 이용 indexing
# print(a[2],"\t",a[0]) # 숫자 인덱스 값 이용 indexing

# # Series 사칙연산
# price_1 = pd.Series([500,1000,2000,4000], index=['2025-06-01','2025-06-02','2025-06-03','2025-06-04'])
# price_2 = pd.Series([1500,2000,1000,3000], index=['2025-06-01','2025-06-02','2025-06-03','2025-06-04'])

# sum = price_1 + price_2; sub = price_1 - price_2
# pro = price_1 * price_2; div = price_1 / price_2

# print(f"Series 간 합 연산\n{sum}", end='\n');print(f"Series 간 차 연산\n{sub}", end='\n')
# print(f"Series 간 곱 연산\n{pro}", end='\n');print(f"Series 간 나누기 연산\n{div}", end='\n')

# # DataFrame 생성 : list-based
# df = pd.DataFrame([
#     [1,2,3],
#     [4,5,6],
#     [7,8,9]
# ])
# print("list-based df",df,sep="\n")

# # DataFrame 생성 : dict-based
# Dict = {
#     'age' : [21,24,23],
#     'height' : [180, 177, 173],
#     'weight' : [80, 70, 59]
# }

# indexName = ['SH','HR','BS']

# df = pd.DataFrame(Dict, index=indexName)
# print("dict-based df",df,sep="\n")

# # DataFrame 데이터 조회 방법
# df = pd.DataFrame({
#     'age' : [21,24,23],
#     'height' : [180, 177, 173],
#     'weight' : [80, 70, 59]},
#     index=['SH','HR','BS'])

# # 행 조회
# print(df.loc['HR'], end='\n')
# print(df.iloc[2])

# # 열 조회
# print(df.height)

# # 원소 조회
# print(f"'SH' 의 키는 : {df.loc['SH'].iloc[1]}") # No FutureWarning
# print(f"'BS' 의 몸무게는 : {df.loc['BS'][2]}") # FutureWarning

# DataFrame slicing
# df = pd.DataFrame({
#     '나이' : [21,24,23],
#     '키' : [180, 177, 173],
#     '몸무게' : [80, 70, 59]},
#     index=['사람1','사람2','사람3'])

# print("loc 으로 slicing\n")
# nameIndex = df.loc['사람1':'사람3']
# print(nameIndex)
# print(type(nameIndex), end='\n')

# print("iloc 으로 slicing\n")
# nameColumn1 = df.iloc[1:3]
# print(nameColumn1)
# print(type(nameColumn1), end="\n")

# DataFrame index 이름과 column 이름 조회
df = pd.DataFrame({
    'age' : [21,24,23],
    'height' : [180, 177, 173],
    'weight' : [80, 70, 59]},
    index=['SH','HR','BS'])

print(df)
print(df.index,end='\n')
print(df.columns)
