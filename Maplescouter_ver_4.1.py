# maple_stats_all_in_one.py (v4.3)
"""
* 전투력·헥사 숫자 추출 로직 개선
* 74 번째 줄 SyntaxError(= 누락) 수정
* 그래프 가독성·headless 환경 저장 기능 추가
"""

from __future__ import annotations

import argparse, importlib, subprocess, sys, re, time, urllib.parse
from dataclasses import dataclass, field
from typing import Dict, Optional

# ────────────────────────── 의존성 자동 설치 ──────────────────────────
REQ = {
    "bs4": "beautifulsoup4",
    "selenium": "selenium",
    "matplotlib": "matplotlib",
    "webdriver_manager": "webdriver-manager",
}
for mod, pip_name in REQ.items():
    try:
        importlib.import_module(mod)
    except ModuleNotFoundError:
        subprocess.check_call([sys.executable, "-m", "pip", "install", pip_name])

from bs4 import BeautifulSoup  # type: ignore
from selenium import webdriver  # type: ignore
from selenium.webdriver.chrome.options import Options  # type: ignore
from selenium.webdriver.common.by import By  # type: ignore
from selenium.webdriver.support.ui import WebDriverWait  # type: ignore
from selenium.webdriver.support import expected_conditions as EC  # type: ignore
from selenium.common.exceptions import InvalidSessionIdException  # type: ignore
from selenium.webdriver.chrome.service import Service  # type: ignore
from webdriver_manager.chrome import ChromeDriverManager  # type: ignore
import matplotlib.pyplot as plt  # type: ignore
import matplotlib.font_manager as fm  # type: ignore

plt.rcParams["axes.unicode_minus"] = False
# 시스템 한글 글꼴 자동 선택
for fp in fm.findSystemFonts():
    if any(k in fp.lower() for k in ("malgun", "nanumgothic", "applegothic")):
        plt.rcParams["font.family"] = fm.FontProperties(fname=fp).get_name()
        break


# ────────────────────────── 데이터 모델 ──────────────────────────
@dataclass
class PlayerStats:
    combat_power: Optional[int] = None
    hexa_stat: Optional[int] = None
    fetched_at: float = field(default_factory=time.time)


# ────────────────────────── 스크래퍼 ──────────────────────────
class MapleScouterScraper:
    BASE = "https://maplescouter.com/info"

    def __init__(self, *, headless: bool = True):
        self.headless = headless
        self._new()

    def _new(self):
        opts = Options()
        if self.headless:
            opts.add_argument("--headless=new")
        opts.add_argument("--no-sandbox")
        opts.add_argument("--disable-dev-shm-usage")
        self.driver = webdriver.Chrome(
            service=Service(ChromeDriverManager().install()), options=opts
        )

    @staticmethod
    def _collect_digits(chunk: str) -> Optional[int]:
        digits = re.sub(r"[^0-9]", "", chunk)
        return int(digits) if len(digits) >= 3 else None

    def _extract_after_label(
        self, text: str, label: str, window: int = 60
    ) -> Optional[int]:
        idx = text.find(label)
        if idx == -1:
            return None
        slice_ = text[idx : idx + window]
        return self._collect_digits(slice_)

    def get_stats(
        self, char: str, preset: str = "00000", timeout: int = 12, retry: bool = True
    ) -> PlayerStats:
        url = f"{self.BASE}?name={urllib.parse.quote(char)}&preset={preset}"
        try:
            self.driver.get(url)
        except InvalidSessionIdException:
            if retry:
                self._new()
                return self.get_stats(char, preset, timeout, False)
            raise
        WebDriverWait(self.driver, timeout).until(
            EC.presence_of_element_located((By.XPATH, "//*[contains(text(),'전투력')]"))
        )
        txt = BeautifulSoup(self.driver.page_source, "html.parser").get_text(" ")
        combat = self._extract_after_label(txt, "전투력")
        hexa = self._extract_after_label(txt, "헥사환산(")
        return PlayerStats(combat_power=combat, hexa_stat=hexa)

    def close(self):
        try:
            self.driver.quit()
        except Exception:
            pass


# ────────────────────────── DB & 시각화 ──────────────────────────
class MapleStatsDB:
    def __init__(self, map_: Dict[str, Optional[str]], headless: bool):
        self.map = map_
        self.headless = headless
        self.scraper: Optional[MapleScouterScraper] = None
        # ← SyntaxError 해결: '=' 로 초기화
        self.cache: Dict[str, PlayerStats] = {}

    # 내부 준비
    def _ready(self):
        if not self.scraper:
            self.scraper = MapleScouterScraper(headless=self.headless)

    @staticmethod
    def _valid(c: Optional[str]) -> bool:
        return bool(c and c.strip() and not c.startswith("<"))

    # 특정 친구 업데이트
    def update_friend(self, f: str) -> PlayerStats:
        c = self.map.get(f)
        if not self._valid(c):
            raise ValueError(f"'{f}' 캐릭터 미등록")
        if f not in self.cache:
            self._ready()
            assert self.scraper
            self.cache[f] = self.scraper.get_stats(c)  # type: ignore[arg-type]
        return self.cache[f]

    # 전원 업데이트
    def update_all(self):
        for f in self.map:
            if self._valid(self.map[f]):
                self.update_friend(f)

    # 그래프
    def plot(self, metric: str = "combat_power"):
        self.update_all()
        data = {
            k: getattr(v, metric)
            for k, v in self.cache.items()
            if getattr(v, metric) is not None
        }
        if not data:
            print("데이터 없음")
            return

        # 내림차순 정렬
        names, values = zip(*sorted(data.items(), key=lambda x: x[1], reverse=True))

        plt.figure(figsize=(10, 6))
        bars = plt.bar(names, values)
        plt.xticks(rotation=30, ha="right")  # 글자 겹침 방지
        plt.ylabel(metric.replace("_", " "))
        plt.title("Friends' " + metric.replace("_", " ").title())
        for b, val in zip(bars, values):
            plt.text(
                b.get_x() + b.get_width() / 2,
                val,
                f"{val:,}",
                ha="center",
                va="bottom",
            )
        plt.tight_layout()
        if self.headless:
            fname = f"{metric}.png"
            plt.savefig(fname)
            print(f"그래프가 '{fname}' 파일로 저장되었습니다.")
        else:
            plt.show()

    def close(self):
        if self.scraper:
            self.scraper.close()


# ────────────────────────── 친구 목록 ──────────────────────────
FRIENDS = {
    "수호": "솔루천국",
    "동건": "여울이숍",
    "건하": "153몰락이렐",
    "민규": "술로밥퍼",
    "양원": None,
    "태현": "",
    "한새": "문한새",
    "부캐": "내누궁지아니",
}

# ────────────────────────── CLI ──────────────────────────
def _menu():
    print(
        "\n===== Maple Stats CLI =====\n"
        "1) 친구 한 명 조회\n"
        "2) 전체 그래프 출력\n"
        "q) 종료"
    )
    return input("선택 >> ").strip().lower()


def _ask_friend():
    print("친구 목록:", ", ".join(FRIENDS.keys()))
    return input("조회할 친구 이름 >> ").strip()


def run(headless: bool):
    db = MapleStatsDB(FRIENDS, headless)
    try:
        while True:
            choice = _menu()
            if choice == "1":
                who = _ask_friend()
                if who not in FRIENDS:
                    print("목록에 없음")
                    continue
                try:
                    s = db.update_friend(who)
                except ValueError as e:
                    print(e)
                    continue
                print(
                    f"{who} → 전투력: {s.combat_power or 'N/A'} | 헥사: {s.hexa_stat or 'N/A'}"
                )
            elif choice == "2":
                db.plot()
            elif choice in {"q", "quit", "exit"}:
                break
            else:
                print("올바른 선택지 입력")
    finally:
        db.close()


# ────────────────────────── 엔트리포인트 ──────────────────────────
if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("--gui", action="store_true", help="GUI 모드(그래프 창 표시)")
    args = parser.parse_args()
    try:
        run(headless=not args.gui)
    except KeyboardInterrupt:
        print("\n[종료]")
