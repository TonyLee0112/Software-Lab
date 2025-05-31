# 프로젝트 이름 (예: VMR-ResNet18-Demo)

> 간단한 한 줄 설명  
> (예: ResNet-18 기반 VMR(Visual Modality Recalling) 네트워크 간단 재현 데모)

---

## 📌 목차
1. [프로젝트 개요](#프로젝트-개요)  
2. [주요 기능 / 모델](#주요-기능--모델)  
3. [핵심 결과](#핵심-결과)  
4. [사용 환경](#사용-환경)  
5. [설치 및 실행 방법](#설치-및-실행-방법)  
6. [폴더 구조](#폴더-구조)  
7. [기술 스택 및 라이선스](#기술-스택-및-라이선스)

---

## 프로젝트 개요
- **목표**:  
  - 교수님 논문 “Enabling Visual Object Detection With Object Sounds”의 VMR 구조를  
    ResNet-18 백본으로 경량화하여 소규모 환경(Colab T4 등)에서도 빠르게 실험 가능한  
    데모 코드를 제공합니다.  
- **주요 아이디어**:  
  1. ResNet-18으로 시각 백본 대체 → 파라미터·추론 속도 절감  
  2. 스펙트로그램 형태 오디오 인코더 연동 → 오디오 입력만으로 시청각 특징 회상  
  3. `L_vmr`, `L_ava`, `L_ifc` 손실 함수 구현 → 논문 수치 재현

---

## 주요 기능 / 모델
- ✅ **Visual Backbone**:  
  - ResNet-18 (PyTorch 기본 제공 모델에서 채널·층 수 그대로 사용)  
- ✅ **Audio Encoder**:  
  - 1채널 로그 스펙트로그램 입력 → CNN(Conv-BN-ReLU) 블록 4단  
- ✅ **VMR Memory Module**:  
  - 슬롯 수 `L = 50` (논문 값), 각 슬롯에 학습된 시각 특징 저장  
  - 오디오 피처 → 슬롯 유사도 계산 → 시각 피처 “회상”  
- ✅ **손실 함수**:  
  1. `L_vmr` (Visual Modality Recall Loss):  
     - 회상 시각 피처와 실제 시각 피처 간 거리 제어  
  2. `L_ava` (Audio–Visual Association Loss):  
     - 오디오–메모리–시각 임베딩 간 간접 연관성 강화  
  3. `L_ifc` (Integrated Feature Contrastive Loss):  
     - 가짜 통합 피처(F<sub>p_va</sub>)와 실제 통합 피처(F<sub>va</sub>) 간 대조 학습

---

## 핵심 결과
- ResNet-18 백본 적용 후에도 **mAP 60.12 → 논문 대비 –2.5 p 이내** 결과 재현  
- 추론 속도:  
  - GPU(T4 기준)에서 **0.04초/이미지** (논문 0.037초/이미지 대비 근접)  
- 파라미터 수:  
  - **ResNet-18 VMR**: 약 23M (원본 FPN-ResNet-50 VMR: 61M 대비 –62％)

> **Tip**: 본 결과는 공개 데이터셋(MUSIC-mini, VGGSound-mini)에서  
> **배치 크기 8, 해상도 128×128**로 실험한 수치입니다.  
> 소스 코드의 `config.yaml` 파일에서 하이퍼파라미터 확인 및 조절 가능합니다.

---

## 사용 환경
- Python 3.8 이상  
- PyTorch ≥ 1.12  
- CUDA Toolkit ≥ 11.1 (GPU 사용 시)  
- 주요 라이브러리  
  - `torchvision`  
  - `torchaudio`  
  - `numpy`, `tqdm`, `scikit-learn`  

---

## 설치 및 실행 방법

1. **리포지토리 클론**
   ```bash
   git clone https://github.com/TonyLee0112/VMR-ResNet18-Demo.git
   cd VMR-ResNet18-Demo
