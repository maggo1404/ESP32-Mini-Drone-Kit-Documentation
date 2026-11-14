// Web flasher UI (works in the site root and in /en/): all URLs are derived from the
// location of this script, which always lives at <site root>/assets/flasher.js.
(function () {
  var host = document.getElementById("flasher");
  if (!host) return;

  var root = (document.currentScript && document.currentScript.src || "")
    .replace(/assets\/flasher\.js(\?.*)?$/, "");
  if (!root) { root = new URL("../", location.href).href; }
  var fw = root + "firmware/";

  var lang = (document.documentElement.lang || "de").slice(0, 2) === "en" ? "en" : "de";
  var T = {
    de: {
      board: "Board / Variante",
      version: "Firmware-Version",
      unknown: "nicht verfügbar (lokaler Build ohne Firmware)",
      unsupported: "Dein Browser unterstützt Web Serial nicht. Nutze Chrome oder Edge am Desktop, oder flashe mit dem Espressif Flash Download Tool bzw. esptool (siehe unten).",
      notAllowed: "Web Serial benötigt eine sichere Verbindung (HTTPS).",
      button: "Firmware installieren",
      files: "Direkt-Downloads",
      merged: "Komplettabbild (merged, Adresse 0x0)",
      app: "Anwendung",
      boot: "Bootloader",
      parts: "Partitionstabelle"
    },
    en: {
      board: "Board / variant",
      version: "Firmware version",
      unknown: "not available (local build without firmware)",
      unsupported: "Your browser does not support Web Serial. Use Chrome or Edge on a desktop computer, or flash with the Espressif Flash Download Tool or esptool (see below).",
      notAllowed: "Web Serial requires a secure connection (HTTPS).",
      button: "Install firmware",
      files: "Direct downloads",
      merged: "Merged image (flash at 0x0)",
      app: "Application",
      boot: "Bootloader",
      parts: "Partition table"
    }
  }[lang];

  var targets = [
    { id: "xw-z1", label: "XW-Z1 (ESP32)", chip: "ESP32" },
    { id: "xw-z1-c3", label: "XW-Z1 C3 (ESP32-C3)", chip: "ESP32-C3" },
    { id: "xw-z1-s3", label: "XW-Z1 S3 (ESP32-S3)", chip: "ESP32-S3" },
    { id: "flix-esp32", label: "Flix stock (ESP32)", chip: "ESP32" }
  ];

  var opts = targets.map(function (t) {
    return '<option value="' + t.id + '">' + t.label + "</option>";
  }).join("");

  host.innerHTML =
    '<p><label for="fl-target"><strong>' + T.board + ':</strong></label> ' +
    '<select id="fl-target">' + opts + "</select></p>" +
    '<p id="fl-button"></p>' +
    '<p><strong>' + T.version + ':</strong> <code id="fl-version">…</code></p>' +
    '<details><summary>' + T.files + '</summary><ul id="fl-files"></ul></details>';

  var sel = host.querySelector("#fl-target");
  var btnBox = host.querySelector("#fl-button");
  var files = host.querySelector("#fl-files");

  function render() {
    var id = sel.value;
    btnBox.innerHTML =
      '<esp-web-install-button manifest="' + fw + "manifest-" + id + '.json">' +
      '<button slot="activate" class="md-button md-button--primary">' + T.button + " (" + id + ")</button>" +
      '<span slot="unsupported" class="admonition warning"><span>' + T.unsupported + "</span></span>" +
      '<span slot="not-allowed">' + T.notAllowed + "</span>" +
      "</esp-web-install-button>";
    var list = [
      ["flix.ino.merged.bin", T.merged],
      ["flix.ino.bin", T.app],
      ["flix.ino.bootloader.bin", T.boot],
      ["flix.ino.partitions.bin", T.parts]
    ];
    files.innerHTML = list.map(function (f) {
      return '<li><a href="' + fw + id + "/" + f[0] + '">' + f[0] + "</a> – " + f[1] + "</li>";
    }).join("");
  }
  sel.addEventListener("change", render);
  render();

  var v = host.querySelector("#fl-version");
  fetch(fw + "version.txt").then(function (r) {
    if (!r.ok) throw new Error(r.status);
    return r.text();
  }).then(function (s) { v.textContent = s.trim(); })
    .catch(function () { v.textContent = T.unknown; });
})();
