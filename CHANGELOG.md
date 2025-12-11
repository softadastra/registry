# Changelog

All notable changes to this project will be documented in this file.

The format is based on **Keep a Changelog**  
and this project adheres to **Semantic Versioning (SemVer)**.

---

## [Unreleased]

### Added

- Initial project structure and full repository architecture.
- Base CMake configuration with presets for dev/prod.
- Draft API documentation (`docs/api.md`).
- Core vision, architecture, and storage model in `docs/`.
- Configuration files: dev, prod, and example local config.
- Database migrations:
  - `0001_init_schema.sql` — packages, versions, users.
  - `0002_add_tokens.sql` — API tokens and scopes.
- Scripts for development workflow (`dev.sh`, `migrate.sh`, `seed.sh`).
- Initial storage system:
  - `LocalFileStorage` implementation.
  - S3 storage stub for future cloud integration.
- Initial HTTP server structure with routes and middleware.
- Domain models: Package, Version, User, Token, Errors.
- Service layer (Auth, Package, Version).
- Repository layer (PackageRepository, UserRepository, Database).
- Minimal working `main.cpp` with App bootstrap.
- Integration and unit tests scaffolding.
- Example clients (Python + Vix client).
- Example cURL API usage.

### Changed

- Improved `.gitignore` with full C++ and project-specific exclusions.
- Clarified registry architecture and API philosophy in docs.

### Removed

- `cmd.md` removed due to sensitive content.

---

## [0.1.0] — WIP

### Planned for first public MVP

- Publish endpoint fully functional.
- Download endpoint serving artifacts.
- Semver-based version resolver.
- Authentication with token scopes (`read`, `publish`).
- First working CLI prototype (`vixpm publish`, `vixpm add`, `vixpm search`).

---

## [0.0.1] — 2025-12-11

### Added

- Repository initialized.
- README.md with core mission and high-level architecture.
