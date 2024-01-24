# C++hess ♟️🎮

Welcome to the C++hess! 🚀
I started this hobby project a long time ago, but I decided now to reboot it with a fresh start. 🌟 I'm on a mission to build a chess engine 🤖 strong enough to beat my friends and, along the way, boost my skills in programming, algorithms, and AI. 🧠💻

It's an ongoing project and any contribution or suggestion is welcome!

## Getting Started 🚦

### Run it yourself! 💻🛠️

To compile C++Chess, you will need `make` and the classic GNU toolchain. Run the `make` in the root directory of the project to compile the source code into an executable.

## Key features 🌟

### Bitboard Move Generation ♟️🔍

C++hess uses bitboards for move generation.
The board is represented as unsigned ints of 64 bits. Bitboards are a highly efficient way to represent the chess board, enabling fast and efficient move generation and board evaluation. 🚀🧩

### Alpha-Beta Pruning and Min-Max Algorithm 🌳✂️

Move generation is based on the alpha-beta pruning algorithm, an optimization technique for the min-max algorithm that significantly reduces the number of nodes evaluated in the search tree, making the engine faster and allowing for a deeper move search.

## Todo List 📝

- [x] FEN format support ✅
- [x] Basic terminal UI ✅
- [x] Initialize project structure ✅
- [x] Non sliding pieces moves generation ✅
- [x] Sliding pieces moves generation (Magic bitboards) ✅
- [ ] From pseudolegal to legal move generation 🔄
- [ ] Evaluation function 🔄
- [ ] Min-max search 🔄
- [ ] Search optimization 🔄
- [ ] GUI 🔄

## License 📜

This project is licensed under the [MIT License](LICENSE).

## Contact 📬

For any queries or contributions, please open an issue or a pull request on the repository.
