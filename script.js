window.addEventListener("DOMContentLoaded", () => {
  const tiles = Array.from(document.querySelectorAll(".tile"));
  const playerDisplay = document.querySelector(".display-player");
  const resetButton = document.querySelector("#reset");
  const announcer = document.querySelector(".display");
  const modeBtns = document.querySelectorAll(".mode-btn");

  let board = ["", "", "", "", "", "", "", "", ""];
  let currentPlayer = "X";
  let isGameActive = true;
  let gameMode = "ai"; // 'ai' hoặc 'two-player'
  let isAIThinking = false;

  const PLAYERX_WON = "PLAYERX_WON";
  const PLAYERO_WON = "PLAYERO_WON";
  const TIE = "TIE";

  /*
        Indexes within the board
        [0] [1] [2]
        [3] [4] [5]
        [6] [7] [8]
    */
  const winningConditions = [
    [0, 1, 2],
    [3, 4, 5],
    [6, 7, 8],
    [0, 3, 6],
    [1, 4, 7],
    [2, 5, 8],
    [0, 4, 8],
    [2, 4, 6],
  ];

  // === AI MINIMAX ALGORITHM ===
  function evaluateBoard(board) {
    for (let condition of winningConditions) {
      const [a, b, c] = condition;
      if (board[a] === "O" && board[b] === "O" && board[c] === "O") {
        return 10; // AI win
      }
      if (board[a] === "X" && board[b] === "X" && board[c] === "X") {
        return -10; // Human win
      }
    }
    return 0; // Draw
  }

  function minimax(board, depth, isMaximizing) {
    const score = evaluateBoard(board);

    // Terminal conditions
    if (score === 10) return score - depth;
    if (score === -10) return score + depth;
    if (!board.includes("")) return 0; // Draw

    if (isMaximizing) {
      let bestScore = -Infinity;
      for (let i = 0; i < 9; i++) {
        if (board[i] === "") {
          board[i] = "O";
          const score = minimax(board, depth + 1, false);
          board[i] = "";
          bestScore = Math.max(score, bestScore);
        }
      }
      return bestScore;
    } else {
      let bestScore = Infinity;
      for (let i = 0; i < 9; i++) {
        if (board[i] === "") {
          board[i] = "X";
          const score = minimax(board, depth + 1, true);
          board[i] = "";
          bestScore = Math.min(score, bestScore);
        }
      }
      return bestScore;
    }
  }

  function getBestMove(board) {
    let bestScore = -Infinity;
    let bestMove = -1;

    for (let i = 0; i < 9; i++) {
      if (board[i] === "") {
        board[i] = "O";
        const score = minimax(board, 0, false);
        board[i] = "";

        if (score > bestScore) {
          bestScore = score;
          bestMove = i;
        }
      }
    }
    return bestMove;
  }

  function aiMove() {
    if (!isGameActive || isAIThinking || currentPlayer !== "O") return;

    isAIThinking = true;
    // Delay AI move slightly để game cảm thấy tự nhiên hơn
    setTimeout(() => {
      const bestMove = getBestMove([...board]);
      if (bestMove !== -1) {
        const tile = tiles[bestMove];
        tile.innerText = "O";
        tile.classList.add("playerO");
        board[bestMove] = "O";
        handleResultValidation();
        if (isGameActive) {
          changePlayer();
        }
      }
      isAIThinking = false;
    }, 500);
  }

  function handleResultValidation() {
    let roundWon = false;
    for (let i = 0; i <= 7; i++) {
      const winCondition = winningConditions[i];
      const a = board[winCondition[0]];
      const b = board[winCondition[1]];
      const c = board[winCondition[2]];
      if (a === "" || b === "" || c === "") {
        continue;
      }
      if (a === b && b === c) {
        roundWon = true;
        break;
      }
    }

    if (roundWon) {
      announce(currentPlayer === "X" ? PLAYERX_WON : PLAYERO_WON);
      isGameActive = false;
      return;
    }

    if (!board.includes("")) announce(TIE);
  }

  const announce = (type) => {
    switch (type) {
      case PLAYERO_WON:
        announcer.innerHTML = 'Player <span class="playerO">O</span> Won!';
        break;
      case PLAYERX_WON:
        announcer.innerHTML = 'Player <span class="playerX">X</span> Won!';
        break;
      case TIE:
        announcer.innerText = "Tie";
    }
  };

  const isValidAction = (tile) => {
    if (tile.innerText === "X" || tile.innerText === "O") {
      return false;
    }
    return true;
  };

  const updateBoard = (index) => {
    board[index] = currentPlayer;
  };

  const changePlayer = () => {
    playerDisplay.classList.remove(`player${currentPlayer}`);
    currentPlayer = currentPlayer === "X" ? "O" : "X";
    playerDisplay.innerText = currentPlayer;
    playerDisplay.classList.add(`player${currentPlayer}`);
  };

  const userAction = (tile, index) => {
    if (
      isValidAction(tile) &&
      isGameActive &&
      currentPlayer === "X" &&
      !isAIThinking
    ) {
      tile.innerText = currentPlayer;
      tile.classList.add(`player${currentPlayer}`);
      updateBoard(index);
      handleResultValidation();

      if (isGameActive) {
        changePlayer();
        // Nếu là chế độ AI, AI sẽ tự động đi
        if (gameMode === "ai") {
          aiMove();
        }
      }
    }
  };

  const resetBoard = () => {
    board = ["", "", "", "", "", "", "", "", ""];
    isGameActive = true;
    isAIThinking = false;
    currentPlayer = "X";
    announcer.innerHTML =
      'Player <span class="display-player playerX">X</span>\'s turn';
    playerDisplay.classList.remove("playerO");
    playerDisplay.innerText = "X";
    playerDisplay.classList.add("playerX");

    tiles.forEach((tile) => {
      tile.innerText = "";
      tile.classList.remove("playerX");
      tile.classList.remove("playerO");
    });
  };

  // Mode selector
  modeBtns.forEach((btn) => {
    btn.addEventListener("click", (e) => {
      modeBtns.forEach((b) => b.classList.remove("active"));
      e.target.classList.add("active");
      gameMode = e.target.dataset.mode;
      resetBoard();
    });
  });

  tiles.forEach((tile, index) => {
    tile.addEventListener("click", () => userAction(tile, index));
  });

  resetButton.addEventListener("click", resetBoard);
});
