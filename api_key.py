import json, os, requests

API_KEY = os.getenv("NEXON_API_KEY","test_080d9c4a69f801f0bb0e508f03d73a07126a3cb0d234ee15b50124e184d330a9efe8d04e6d233bd35cf2fabdeb93fb0d")
HEADERS = {"x-nxopen-api-key" : API_KEY}

# getOCID
character_name = "솔루천국"
RequestURL = "https://open.api.nexon.com/maplestory/v1/id?character_name=%EC%86%94%EB%A3%A8%EC%B2%9C%EA%B5%AD"
response = requests.get(RequestURL, headers=HEADERS, params={"character_name": character_name})
response.raise_for_status()
ocid = response.json()["ocid"]

# Equipped Item info


# Character Level



# 유저들 리스트 {"실제 이름" : "캐릭터 닉네임"}
users = {"수호" : "땀수급", "동건" : "여울급",
        "양원" : "찐메련", "창우" : "민초창우",
        "건하" : "153몰락이렌", "태현" : "희보렌",
        "서준" : "오란렌", "현수" : "타고싶다", "민규" : "리민큐알코드"}


# 전투력 순위 그래프



# 캐릭터 레벨 그래프


# Automated Daily update

