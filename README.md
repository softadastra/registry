# Softadastra Registry

A universal, offline‑first, high‑performance package registry powering the entire Softadastra ecosystem — including Vix.cpp, Ivi.php, Rix, Softadastra Drive, and WorldNet applications.

## 🚀 Vision

The **Softadastra Registry** is the foundation of the upcoming WorldNet infrastructure.  
It provides a **secure, global, low‑latency, distributed package registry** designed for environments with unstable connectivity such as many African regions.

Its mission is to be:

- **Universal** — supports packages for Vix.cpp, Ivi.php, Rix, extensions, plugins, and future Softadastra apps.
- **Offline‑first** — integrates naturally with Softadastra Drive for local mirroring and caching.
- **Distributed** — ready for peer‑to‑peer sync later via WorldNet nodes.
- **Fast & secure** — implemented using modern C++ and optimized server‑side logic.

This is the core building block that allows developers to build, distribute, and install software across the entire Softadastra stack.

---

## 🏗 Architecture Overview

The registry backend follows a clean layered architecture:

```
HTTP Layer (API, routing, middleware)
        ↓
Services (publish, resolve, search, auth)
        ↓
Domain (Package, Version, User, Token)
        ↓
Persistence Layer (MySQL/Postgres repositories)
        ↓
Storage (local filesystem → S3/Drive mirror → P2P later)
```

### Main components

- **HTTP Server** — powered by Vix.cpp
- **Authentication** — token‑based access with scopes
- **Publish Pipeline** — receives package metadata + tarball
- **Resolver** — resolves versions using semver rules
- **Search Engine** — index + filters (simple at first)
- **Storage System**
  - v0 → Local filesystem
  - v1 → Softadastra Drive mirror
  - v2 → WorldNet distributed nodes

---

## 📦 API Overview (Draft)

### `POST /api/packages/publish`

Publish a new package or version.  
Requires authentication token.

### `GET /api/packages/:name`

Returns package metadata (description, versions, owner…).

### `GET /api/packages/:name/:version/download`

Returns the binary artifact (`.tar.gz / .zip / .whl / .vixpkg`).

### `GET /api/search?q=...`

Basic search on name + tags.

Full API documented in `docs/api.md`.

---

## 📂 Repository Structure

```
registry/
├── README.md
├── CHANGELOG.md
├── LICENSE
├── CMakeLists.txt
├── config/
├── docs/
│   ├── architecture.md
│   ├── api.md
│   ├── storage.md
│   ├── security.md
│   └── roadmap.md
├── migrations/
├── scripts/
├── infra/
├── include/
│   └── softadastra/registry/
├── src/
├── tests/
```

---

## 🔥 MVP Roadmap (v0.1)

### ✅ Phase 1 — Core foundation

- Project scaffolding
- CMake + build system
- Minimal HTTP server (Vix.cpp)
- Routing structure
- Local filesystem storage

### 🚧 Phase 2 — Package publishing

- Manifest validation
- Tarball upload
- Integrity hash
- DB schema for packages + versions

### 🚧 Phase 3 — Package resolution

- Semver resolver
- Download endpoint

### 🚧 Phase 4 — Authentication & permissions

- Token generation
- Scopes: `read`, `publish`, `admin`

### 🚧 Phase 5 — CLI integration (`vixpm`)

- `vixpm publish`
- `vixpm add <package>`
- `vixpm search`

---

## 🌍 Future: Softadastra Drive + P2P

The registry is designed to integrate with:

### Softadastra Drive

- Local mirroring
- Offline installation
- Sync acceleration in low‑bandwidth regions

### Softadastra net P2P

- Distributed registry nodes
- CRDT‑based version propagation
- High availability even without cloud connectivity

---

## 🤝 Contributing

The project follows a clean modular structure.  
PRs for API improvements, code quality, or additional storage backends are welcome.

---

## 📜 License

MIT License — free to use, modify and distribute.

---

## ✨ Maintained by

**Softadastra Group — Global OS + WorldNet**  
Lead developer: Gaspard Kirira (https://x.com/@g_kirira)
