# LSWebp 虚幻引擎插件

## 简介
**LSWebp** 是一个用于虚幻引擎（Unreal Engine）的 Runtime 插件。该插件提供了一套完整的蓝图接口，允许开发者在游戏运行时录制视口（Viewport）画面并将其导出为 `.webp` 格式的动画图片，同时还支持在游戏内动态加载和播放本地的 WebP 图像文件。

## 主要特性
* **灵活的区域录制**：支持录制完整的游戏视口，也支持通过自定义坐标 (X0, Y0) 到 (X1, Y1) 来截取特定区域的画面。
* **异步生成**：导出 WebP 文件时采用异步处理，防止在生成长图或大图时阻塞游戏主线程，并提供了完成时的委托回调（Delegate）。
* **动态加载与播放**：支持在游戏运行时通过绝对路径加载外部的 `.webp` 文件，并将其转换为随时间动态更新的 `UTexture2D` 以供 UI 或材质使用。
* **内存安全**：提供了专门的资源释放接口，确保在不使用 WebP 时安全释放内存。

## 蓝图 API 概览
插件包含以下核心蓝图节点：
* `BeginRecord`：开始录制视口中指定像素范围的画面。
* `BeginRecordFullViewport`：开始录制完整的视口画面。
* `EndRecord`：结束录制并异步生成 WebP 文件，完成后触发回调。
* `LoadWebp`：加载并播放 WebP 文件，实时返回更新的 `UTexture2D` 以及画面的宽和高。
* `ReleaseLoadedWebp`：释放已加载的 WebP 资源，避免内存泄漏。

---

## 蓝图配置与使用示例

**操作说明：**
项目在关卡蓝图中绑定键盘按键1来触发录制。录制分为全屏录制和自定义尺寸录制，调用 `BeginRecordFullViewport` 节点是开始捕捉当前视口的所有画面，`BeginRecord`则是可以自定义视口画面尺寸。
* **注意**：请确保保存路径以 `.webp` 结尾。

**蓝图截图：**
开始录制全屏蓝图
![开始录制全屏逻辑](https://github.com/user-attachments/assets/d5197868-da36-4e97-9d9e-be04d765a26f)
开始录制选取范围画面蓝图
![开始录制选定范围逻辑](https://github.com/user-attachments/assets/c51a73bb-cbc5-4349-b493-8463bb8c9d3d)

### 2. 结束录制 (End Record)
**操作说明：**
当按下键盘按键2时，调用 `EndRecord` 节点。该节点会停止捕捉并开始异步合成 WebP 文件。

**蓝图截图：**
结束录制蓝图
![结束录制逻辑](https://github.com/user-attachments/assets/5faa8230-da1d-416d-bcd5-36d11058f000)


### 3. 加载录制 (End Record)
**配置说明：**
在创建设置好材质以及在地图上生成好材质之后，可以按下键盘按键3来调用`LoadRecord` 节点以及动态生成材质纹理让材质调用之前录制好的画面。


**蓝图截图：**

![材质设置](https://github.com/user-attachments/assets/189a1a5f-ab7f-4933-a255-ff971bd30e58)

![加载录制画面逻辑](https://github.com/user-attachments/assets/c99e00bd-1500-4fcf-8383-222a3be3d996)

---

## 注意事项
1. **保存路径要求**：在调用录制功能时，传入的保存路径必须是有效的目录（不可为磁盘根目录），且文件扩展名必须以 `.webp` 结尾。
2. **异步回调**：`EndRecord` 生成图片的过程可能较为耗时，请务必依赖回调事件来执行后续逻辑。
3. **视口坐标系**：使用部分区域录制功能时，请注意视口坐标从左上角 `(0,0)` 开始计算。
