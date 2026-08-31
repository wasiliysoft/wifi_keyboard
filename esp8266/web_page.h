const char HTML_PAGE[] PROGMEM = R"rawliteral(

<!DOCTYPE html>
<html lang="ru">

<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>DebugBoard</title>
  <style>
    body {
      font-family: sans-serif;
      background: #f4f4f9;
      color: #333;
      margin: 10px;
    }

    h2,
    h3,
    h4,
    h5 {
      color: #444;
      margin-top: 5px;
    }

    .container {
      max-width: 900px;
      margin: 0 auto;
      background: white;
      padding: 15px;
      border-radius: 8px;
      box-shadow: 0 2px 4px rgba(0, 0, 0, 0.1);
    }

    input[type="text"],
    textarea {
      width: 100%;
      padding: 12px;
      box-sizing: border-box;
      margin-bottom: 10px;
      border: 1px solid #ccc;
      border-radius: 4px;
      font-size: 16px;
    }

    textarea {
      width: 100%;
      min-height: 40px;
      field-sizing: content;
      resize: none;
    }

    button {
      background: #007bff;
      color: white;
      border: none;
      padding: 12px 15px;
      border-radius: 4px;
      cursor: pointer;
      font-size: 15px;
      width: 100%;
      -webkit-tap-highlight-color: transparent;
    }

    button:hover {
      background: #0056b3;
    }

    .tiles-grid {
      display: grid;
      grid-template-columns: repeat(3, 1fr);
      gap: 8px;
      margin-top: 5px;
    }

    /* Плитка относительный контейнер для позиционирования бейджа */
    .tile {
      position: relative;
      background: #e9ecef;
      border: 1px solid #dee2e6;
      padding: 25px 20px 15px 20px;
      border-radius: 6px;
      text-align: left;
      cursor: pointer;
      user-select: none;
      font-weight: bold;
      overflow: hidden;
      text-overflow: ellipsis;
      white-space: nowrap;
      font-size: 16px;
      transition: background 0.15s ease;
      -webkit-tap-highlight-color: transparent;
    }

    .tile:active {
      background: #28a745 !important;
      color: white;
      border-color: #28a745;
    }

    /* Активное состояние — плитка подсвечена во время запроса */
    .tile.active-click {
      background: #28a745 !important;
      color: white;
      border-color: #28a745;
    }

    /* Стили для бейджа комментария в углу */
    .tile-badge {
      position: absolute;
      top: 4px;
      right: 6px;
      font-size: 11px;
      color: #777;
      background: #dbdfe2;
      padding: 1px 6px;
      border-radius: 4px;
      font-weight: normal;
      max-width: 50%;
      overflow: hidden;
      text-overflow: ellipsis;
      white-space: nowrap;
    }

    .tile.active-click .tile-badge {
      color: #28a745;
      background: white;
    }

    /* Стили для разделителя #help */
    .section-divider {
      grid-column: 1 / -1;
      background: #f0ece2;
      color: #4a5568;
      border-left: 4px solid hsl(42, 100%, 50%);
      padding: 8px 12px;
      margin-top: 10px;
      margin-bottom: 2px;
      border-radius: 4px;
      font-size: 14px;
      font-weight: bold;
      letter-spacing: 0.5px;

      /* Свойства для липкости */
      position: sticky;
      top: 0;
      z-index: 10;
    }

    .nav-tabs {
      display: flex;
      gap: 5px;
      margin-bottom: 15px;
      border-bottom: 2px solid #dee2e6;
      padding-bottom: 8px;
    }

    .nav-tabs button {
      background: #6c757d;
      font-size: 13px;
      padding: 8px 10px;
      width: auto;
      flex: 1;
    }

    .nav-tabs button.active-tab {
      background: #007bff;
      font-weight: bold;
    }

    .hidden {
      display: none;
    }

    /* ========== СТАТУС В НИЖНЕМ ЛЕВОМ УГЛУ ========== */
    #status {
      position: fixed;
      bottom: 16px;
      left: 16px;
      margin: 0;
      padding: 6px 14px;
      background: rgba(0, 0, 0, 0.75);
      color: #fff;
      font-size: 13px;
      border-radius: 20px;
      font-weight: 500;
      z-index: 9999;
      backdrop-filter: blur(4px);
      box-shadow: 0 2px 8px rgba(0, 0, 0, 0.2);
      letter-spacing: 0.3px;
      pointer-events: none;
      user-select: none;
      transition: background 0.2s;
    }

    #status.connected {
      background: rgba(40, 167, 69, 0.85);
    }

    #status.disconnected {
      background: rgba(220, 53, 69, 0.85);
    }
  </style>
</head>

<body>
  <div class="container">
    <div class="nav-tabs">
      <button id="btn-main" onclick="switchTab('main')" class="active-tab">Заготовки</button>
      <button id="btn-input" onclick="switchTab('input')">Ввод текста</button>
      <button id="btn-settings" onclick="switchTab('settings')">Настройки</button>
    </div>

    <!-- ВКЛАДКА 1: ПЛИТКИ -->
    <div id="tab-main">
      <div class="tiles-grid" id="tiles-container"></div>
    </div>

    <!-- ВКЛАДКА 2: СВОБОДНЫЙ ВВОД -->
    <div id="tab-input" class="hidden">
      <h3>Свободный ввод текста</h3>
      <input type="text" id="text_in" placeholder="Введите латиницу или цифры..." autocomplete="off">
      <button onclick="sendFreeText()">Отправить на клавиату</button>
    </div>

    <!-- ВКЛАДКА 3: НАСТРОЙКА -->
    <div id="tab-settings" class="hidden">
      <p>Формат файла настроек</p>
      <p style="font-size:12px; color:#666; margin-bottom: 10px;">
        #help Название раздела<br />
        текст для отправки на клавиатуру #comment текст комментария<br />
        // текст который не нужно отображать
      </p>
      <br />
      <div>
        <input type="file" id="tiles-file" class="hidden">
        <button for="tiles-file" onclick="document.getElementById('tiles-file').click();">Выбрать файл макросов</button>
      </div>
    </div>
  </div>

  <!-- СТАТУС В НИЖНЕМ ЛЕВОМ УГЛУ -->
  <div id="status">Проверка подключения...</div>

  <script>
    const targetUrl = '/';
    async function checkUrl(url) {
      try {
        const response = await fetch(url, { method: 'HEAD', mode: 'no-cors' });
        return response.ok || response.type === 'opaque';
      } catch {
        return false;
      }
    }

    async function updateStatus() {
      const statusEl = document.getElementById('status');
      const isAlive = await checkUrl(targetUrl);
      if (isAlive) {
        statusEl.textContent = '● Подключено';
        statusEl.className = 'connected';
      } else {
        statusEl.textContent = '○ Нет подключения';
        statusEl.className = 'disconnected';
      }
    }

    setInterval(updateStatus, 1000);
    updateStatus();
  </script>

  <script>
    let vboard_tiles = [];

    document.getElementById('tiles-file').onchange = function () {
      var rd = new FileReader();
      rd.onload = function (event) {
        parseTiles(event.target.result);
      }
      rd.readAsText(event.target.files[0]);
    }

    function switchTab(tab) {
      document.getElementById('tab-main').classList.add('hidden');
      document.getElementById('tab-input').classList.add('hidden');
      document.getElementById('tab-settings').classList.add('hidden');
      document.getElementById('btn-main').classList.remove('active-tab');
      document.getElementById('btn-input').classList.remove('active-tab');
      document.getElementById('btn-settings').classList.remove('active-tab');

      if (tab === 'main') {
        document.getElementById('tab-main').classList.remove('hidden');
        document.getElementById('btn-main').classList.add('active-tab');
        renderTiles();
      } else if (tab === 'input') {
        document.getElementById('tab-input').classList.remove('hidden');
        document.getElementById('btn-input').classList.add('active-tab');
        setTimeout(() => document.getElementById('text_in').focus(), 100);
      } else if (tab === 'settings') {
        document.getElementById('tab-settings').classList.remove('hidden');
        document.getElementById('btn-settings').classList.add('active-tab');
      }
    }

    function parseTiles(text) {
      const lines = text.split('\n').map(line => line.trim()).filter(line => line.length > 0);

      for (let i = 0; i < lines.length; i++) {
        let line = lines[i];
        if (line.startsWith('#help')) continue;

        let macroPart = line;
        if (line.includes('#')) {
          macroPart = line.substring(0, line.indexOf('#')).trim();
        }

        const invalidChar = findInvalidChar(macroPart);
        if (invalidChar !== null) {
          alert(`Ошибка в строке ${i + 1}: В коде макроса найдена кириллица или недопустимый символ "${invalidChar}"\nСохранение отменено.`);
          return;
        }
      }

      vboard_tiles = lines;
      switchTab('main');
    }

    function renderTiles() {
      const container = document.getElementById('tiles-container');
      container.innerHTML = '';
      const rawData = vboard_tiles;

      if (!rawData || rawData.length === 0) {
        container.innerHTML = '<p style="color:#999; font-size:14px; text-align:center; padding: 20px;">Список пуст. Добавьте заготовки во вкладке "Настройки".</p>';
        return;
      }

      const lines = rawData;
      lines.forEach(line => {
        if (line.startsWith('#help')) {
          const dividerText = line.substring(5).trim();
          const div = document.createElement('div');
          div.className = 'section-divider';
          div.innerText = dividerText;
          container.appendChild(div);
          return;
        }

        if (line.startsWith('//')) {
          return;
        }

        let macroText = line;
        let commentText = '';

        if (line.includes('#comment ')) {
          const index = line.indexOf('#comment ');
          macroText = line.substring(0, index).trim();
          commentText = line.substring(index + 9).trim();
        }

        const div = document.createElement('div');
        div.className = 'tile';
        div.innerText = macroText;
        div.title = macroText;

        if (commentText) {
          const badge = document.createElement('span');
          badge.className = 'tile-badge';
          badge.innerText = commentText;
          div.appendChild(badge);
        }

        div.onclick = () => sendTileText(macroText, div);
        container.appendChild(div);
      });
    }

    function sendFreeText() {
      const inputField = document.getElementById('text_in');
      const text = inputField.value;
      if (text.length > 0) {
        // Для свободного ввода используем синхронную отправку без подсветки
        if (validateAndSend(text)) {
          inputField.value = '';
        }
      }
    }

    // ========== НОВАЯ ЛОГИКА ПОДСВЕТКИ ==========
    async function sendTileText(text, element) {
      // 1. Проверяем валидность перед отправкой
      const invalidChar = findInvalidChar(text);
      if (invalidChar !== null) {
        alert(`Недопустимый символ для отправки: "${invalidChar}"\nРазрешена только латиница, цифры и спецсимволы.`);
        return;
      }

      // 2. Включаем подсветку
      element.classList.add('active-click');

      try {
        // 3. Отправляем запрос и ждём ответ
        const response = await fetch('/send', {
          method: 'POST',
          headers: { 'Content-Type': 'text/plain' },
          body: text
        });

        // 4. Проверяем, что запрос успешен (опционально)
        if (!response.ok) {
          console.warn('Ответ от сервера:', response.status);
        }
      } catch (error) {
        console.error('Ошибка отправки:', error);
      } finally {
        // 5. ВСЕГДА убираем подсветку после завершения запроса
        element.classList.remove('active-click');
      }
    }

    function findInvalidChar(text) {
      for (let i = 0; i < text.length; i++) {
        const code = text.charCodeAt(i);
        if (code < 32 || code > 126) {
          return text.charAt(i);
        }
      }
      return null;
    }

    // Функция validateAndSend используется только для свободного ввода (без подсветки)
    function validateAndSend(text) {
      const invalidChar = findInvalidChar(text);
      if (invalidChar !== null) {
        alert(`Недопустимый символ для отправки: "${invalidChar}"\nРазрешена только латиница, цифры и спецсимволы.`);
        return false;
      }

      // Отправляем без ожидания ответа
      fetch('/send', {
        method: 'POST',
        headers: { 'Content-Type': 'text/plain' },
        body: text
      });
      return true;
    }

    switchTab('settings');
  </script>
</body>

</html>


)rawliteral";
