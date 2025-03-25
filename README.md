# project_pkmbattle
부경대학교 IOT 개발자과정 강황석준조 미니프로젝트 저장소

## 개발 환경설정

- vcpkg 및 SFML 설치
    ```shell
    ## C:/Dev/ 로 cd 한 후
    git clone https://github.com/microsoft/vcpkg.git

    cd vcpkg
    .\bootstrap-vcpkg.bat

    .\vcpkg integrate install   ## visual studio 연동

    vcpkg install SFML:x64-windows     ## SFML 설치
    ```

## 1차 구현목표

- Client
    - SFML 통한 렌더링

    - 캐릭터 움직임과 상호작용 구현하기.

- Server
    - Client 소켓 프로그래밍
    - DB 연동 

- DB
    - 스키마 생성 및 DB 구조
