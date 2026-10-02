# Context for continuing this session

Paste this into a new chat, together with `c-mastery-roadmap.md` (the full plan).

## About me

- **Name:** Karan Rajput. B.Tech ECE, Anand Engineering College, Agra (2022–2026), CGPA 7.70.
- **Job:** Embedded Systems & IoT Developer (SDE-1) at VideoSDK.live since Feb 2026, after an internship there from Aug 2025 to Jan 2026. Take-home pay about ₹34k/month (roughly ₹4.5–5 LPA CTC).
- **Work there:** a real-time IoT SDK for low-latency audio and voice interaction with AI agents (ESP32-S3, Jetson Orin Nano, Raspberry Pi). Also an SO-101 robot arm: teleoperation, dataset collection, fine-tuning and inference of VLA models (SmolVLA, Pi0, MolmoAct) on Jetson Orin and cloud GPUs. I've been doing this ML work for a few months.
- **Company rule:** I can't post anything about company work publicly.
- **Skills:** STM32F4, ESP32, Arduino; UART, I2C, SPI, CAN, LoRa; FreeRTOS; MQTT, HTTP, WebSocket, WHIP/WHEP; bare-metal GPIO and TIM2 drivers on STM32F446. Drone assembly. One IRJET paper. Innovation Bharat winner (2023), Smart India Hackathon grand finalist (2024).
- **Honest language level:** C is my only real language, and even there I couldn't write a ring buffer from scratch. My C++, Python and Rust work was done with AI tools (Claude Code), not on my own. I'm stronger at concepts than at writing code.
- **Setup:** MacBook with Apple Silicon (M2), using `lldb`. Boards: STM32F446 (assumed to be a Nucleo-F446RE, not confirmed) and ESP32 / ESP32-S3.

## Career decisions made so far

- **Question I asked:** stay in core embedded, or move into physical AI (robotics / robot learning)?
- **Conclusion:** don't pick one. Build the career on embedded, and use physical AI as the edge. Target roles like robotics embedded engineer, edge AI deployment, or firmware at a robotics company. Pure ML / robot-learning research roles aren't realistic yet, because those interviews test Python, PyTorch and ML concepts live.
- **Indian physical AI startups discussed:** Addverb, Ati Motors, CynLr, Perceptyne, General Autonomy, Unbox Robotics, Peer Robotics, Flux Auto, Innefu Labs, Human Archive, and others. Most are in Bengaluru, Noida and Hyderabad.
- **Salary context (rough):** embedded freshers about ₹4–8 LPA; robotics software about ₹7–13 LPA; I'm at the lower end for my skills.
- **Resume gaps identified:** no Python, PyTorch or ROS2 listed; the SO-101 work is buried in a single bullet; no public proof of work.
- **Company restriction workaround:** build personal projects at home or in simulation, never reuse company code or data, describe company work on the resume in general terms, contribute to open source (e.g. LeRobot).
- **Language order:** C first (current focus), then Python, then C++. Rust is optional and later.

## The C learning plan

- The full plan is in `c-mastery-roadmap.md`. It's project-based: one system, **SensorHub**, built across 9 stages plus an STM32 warm-up track.
- **SensorHub:** a sensor node on the STM32F446 that streams filtered temperature readings over UART as 13-byte frames, plus a laptop tool (`hub`) that decodes, stores and analyses them. The ESP32 appears as an optional Wi-Fi gateway capstone in Stage 8.
- **Why project-based:** I forget concepts learned from theory or small drills because I never use them again. In this plan, every module is reused by later stages.
- **Rules:** no AI writes code in the repo (AI may review afterwards); test or predict before code; 20 minutes of my own debugging before asking; commit every session; log every bug in `BUGS.md`; a "rebuild day" every 4th session.
- **Why STM32 and not ESP32 for Stages 4–5:** Cortex-M internals (registers, vector table, NVIC, PendSV, linker scripts) are easy to reach on the STM32 and hidden by ESP-IDF on the ESP32.

## Where I am now

- **Repo:** `sensor_hub/` with folders `common/`, `firmware/`, `host/`, `lab/`, `tests/`. `BUGS.md` may not exist yet.
- **Stage 0:** done.
- **Stage 1 (packet decoder CLI):** in progress. I'd only created the folder structure; now I'm writing the actual files.
- **Next files, in order:** `common/protocol.h` → `host/gen.c` → `tests/data/expected.txt` → `common/protocol.c` → `host/format.c` → `host/main.c`.

### Stage 1 protocol (13-byte frame)

| Bytes | Field | Type | Notes |
| --- | --- | --- | --- |
| 0 | sync | `uint8_t` | always `0xAA` |
| 1 | kind + flags | `uint8_t` | high nibble = sensor kind; bit 0 = byte order (0 little, 1 big); bit 1 = calibrated |
| 2–3 | temperature | `int16_t` | centi-degrees (−1250 = −12.50 °C) |
| 4–7 | device id | `uint32_t` | |
| 8–11 | timestamp | `uint32_t` | ms since boot |
| 12 | checksum | `uint8_t` | sum of bytes 1–11, wrapping |

### Stage 1 file roles

| File | Role |
| --- | --- |
| `common/protocol.h` | Constants, flag masks, `packet_t`, result codes, decode/checksum declarations. Needs header guards. |
| `common/protocol.c` | Checks a frame and fills a `packet_t` through a pointer (bytes → struct). Never prints. |
| `host/gen.c` | Standalone program. Builds test frames by hand (values → bytes), including broken ones, and writes `edge.bin`. |
| `host/main.c` | `hub` entry point: reads arguments, reads bytes (file or `--hex`), calls the decoder, calls the printer. |
| `host/format.c/.h` | Text output: temperature formatter (sign trap), frame line, error messages. |
| `tests/data/edge.bin` | Written by `gen`, never edited by hand. |
| `tests/data/expected.txt` | My predicted output, written on paper before running; checked with `diff`. |

### Decisions already made in Stage 1

- **My first `packet_t` draft had problems, and I was told:** the typedef needs its name after the closing brace; temperature must be `int16_t` (signed), not `uint16_t`; fix the spelling "temprature"; don't store the sync byte (the decoder checks it); split byte 1 into separate `kind` and `flags` members; storing the checksum is optional. After it compiles, predict `sizeof(packet_t)` on paper, then reorder members and predict again.
- **`gen.c`** is standalone (its own `main`) and lives in `host/`, not `lab/`. It may use `packet_t` to hold the values of each test frame, but it must split fields into bytes itself, never using the decoder or encoder.
- **The struct is filled by the decoder** in `protocol.c`, via a pointer to a `packet_t` declared in `main.c`.
- **Includes:** write `#include "protocol.h"` (quotes) and add `-Icommon` to the build line. Don't use `<protocol.h>` or `"../common/protocol.h"`.
- **No CMake yet.** A handwritten Makefile and CMake come in Stage 6. For now, plain build lines, maybe saved in a small shell script.
- **Build line** (run from `sensor_hub/`; executables go in `build/`, which is in `.gitignore`):

```sh
gcc -std=c11 -Wall -Wextra -Werror -Wshadow -Wconversion -g \
    -fsanitize=address,undefined \
    -Icommon host/gen.c common/protocol.c -o build/gen
```

- On the Mac, `gcc` is actually Apple clang. Valgrind doesn't run on Apple Silicon: use ASan, plus `leaks --atExit -- ./program` for leaks.

## How I want help

- **Don't write my code.** Give ideas, hints, file roles and design guidance. Reviewing code I've written is fine.
- Keep explanations simple and short, and give direct yes/no answers when I ask for them.
- Use tables when I ask for them.
- Give me documents as simple `.md` files in the chat. No Claude Docs or published artifacts.