# 模块索引（请持续维护）

> 目标：让 AI 助手每次只加载你指定的模块目录，避免全仓库冗余上下文。

---

## 使用方式

1. **给每个模块建立独立目录**（如 `auto-screenshot/`）。
2. **模块目录下放置 `AGENTS.md` 或 `CONTEXT.md`**，写清楚：
   - 模块用途
   - 入口文件
   - 关键依赖
   - 需要加载的最小文件集合
3. **在本文件登记模块索引**（见下方模板）。

下次对话时，直接告诉 AI：
> “只加载 `<模块路径>` 目录，优先读该目录的 AGENTS/CONTEXT。”

---

## 模块索引模板

> 复制后按需补充。

```
- name: auto-screenshot
  path: /workspace/ISP/ISP/auto-screenshot
  entry:
    - src/index.ts
    - src/screenshot.ts
  context_files:
    - AGENTS.md
    - CONTEXT.md
  notes: 自动截图/渲染/存储模块
```

---

## 建议的 AGENTS/CONTEXT 模板

### AGENTS.md

```
# Module: auto-screenshot

Scope: /workspace/ISP/ISP/auto-screenshot

## What to load
- Only files under this directory are relevant.
- If extra context is needed, read CONTEXT.md first.
```

### CONTEXT.md

```
# 自动抓图模块上下文

## 作用
自动截图/渲染/存储

## 入口文件
- src/index.ts
- src/screenshot.ts

## 关键依赖
- playwright
```
