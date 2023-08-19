#define _CRT_SECURE_NO_WARNINGS

#include "Main.h"

int main()
{
	setlocale(LC_ALL, "Rus");
	// инициализируем GLFW
	glfwInit();
	// Тут мы говорим OpenGL какую ему использовать версию
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Тут мы говорим OpenGL какую ему использовать версию
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Создаем окно: ширина, высота, название, полноэкранный режим( glfwGetPrimaryMonitor() вместо NULL), хз чё
	GLFWwindow* window = glfwCreateWindow(widthResolution, hieghtResolution, "alt + f4 to win", NULL, NULL);
	if (window == NULL)
	{
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// Тут мы говорим OpenGL что он должен открыть окно которое мы создали
	glfwMakeContextCurrent(window);

	// Тут мы загружаем глад который помогает нам настроить OpenGL
	gladLoadGL();

	// настраиваем viewport 
	// https://learn.microsoft.com/en-us/windows/win32/opengl/glviewport
	glViewport(0, 0, widthResolution, hieghtResolution);

	// инициализируем imgui и implot
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImPlot::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); 
	(void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	// Основной цикл программы
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		// чистми back buffer и связываем его с новым цветом
		glClear(GL_COLOR_BUFFER_BIT);

		// говорим OpenGL что скоро будет новый кадр
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		// ImGUI window creation
		ImGui::Begin("Okno");
		//--------------------------------------------------------------------------------------------------------------------

		ImGui::BeginChild("fst");
		renderPlot();
		ImGui::EndChild();

		ImGui::Separator();
		ImGui::Text("\n");

		ImGui::BeginChild("fst");
		renderPlot_with_multi_axis();
		ImGui::EndChild();

		//ImPlot::ShowDemoWindow();

		//--------------------------------------------------------------------------------------------------------------------
		// ends the window
		ImGui::End();

		// рендер элементов imgui
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// меняем back buffer и front buffer местами
		glfwSwapBuffers(window);
		// выполняем все GLFW ивенты
		glfwPollEvents();
	}

	// жестко удаляем приколы imgui и implot
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImPlot::DestroyContext();
	ImGui::DestroyContext();

	// удаляем окно и закрывем GLFW
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}

// helper function to load an image into a OpenGL texture with common settings
bool LoadTextureFromFile(const char* filename, GLuint* out_texture, int* out_width, int* out_height)
{
	// Load from file
	int image_width_1 = 0;
	int image_height_1 = 0;
	unsigned char* image_data = stbi_load(filename, &image_width_1, &image_height_1, NULL, 4);
	if (image_data == NULL)
		return false;

	// Create a OpenGL texture identifier
	GLuint image_texture;
	glGenTextures(1, &image_texture);
	glBindTexture(GL_TEXTURE_2D, image_texture);

	// Setup filtering parameters for display
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // This is required on WebGL for non power-of-two textures
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Same

	// Upload pixels into texture
#if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
	glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#endif
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width_1, image_height_1, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
	stbi_image_free(image_data);

	*out_texture = image_texture;
	*out_width = image_width_1;
	*out_height = image_height_1;

	return true;
}


// много умных действий c implot
void renderPlot() {
	static float xs1_1[1001], ys1_1[1001];
	for (int i = 0; i < 1001; ++i) {
		xs1_1[i] = i * 0.001f;
		ys1_1[i] = 0.0f + 0.5f * sinf(50 * (xs1_1[i] + (float)ImGui::GetTime() / 10));
	}
	static double xs2_1[20], ys2_1[20];
	for (int i = 0; i < 20; ++i) {
		xs2_1[i] = i * 1 / 19.0f;
		ys2_1[i] = xs2_1[i] * xs2_1[i];
	}
	if (ImPlot::BeginPlot("Line Plots")) {
		ImPlot::SetupAxes("x", "y");
		ImPlot::PlotLine("f(x)", xs1_1, ys1_1, 1001);
		ImPlot::SetNextMarkerStyle(ImPlotMarker_Circle);
		ImPlot::PlotLine("g(x)", xs2_1, ys2_1, 20, ImPlotLineFlags_Segments);
		ImPlot::EndPlot();
	}
}

// много умных действий но только с imgui
void imguiPlot() {
	static float someGraph[] = { 0.02f, 1.0f, 1.0f, 0.005f, 0.2f, 0.99f, 0.2f };

	ImGui::Text("krutoy graphic");
	ImGui::PlotLines("Frame Times", someGraph, IM_ARRAYSIZE(someGraph));

	ImGui::Text("graphic pokruche");

	static bool animate = true;
	ImGui::Checkbox("Animate", &animate);

	static float values[90] = {};
	static int values_offset = 0;
	static double refresh_time = 0.0;

	if (!animate || refresh_time == 0.0) {
		refresh_time = ImGui::GetTime();
	}

	while (refresh_time < ImGui::GetTime()) // Create data at fixed 60 Hz rate 
	{
		static float phase = 0.0f;
		values[values_offset] = cosf(phase);
		values_offset = (values_offset + 1) % IM_ARRAYSIZE(values);
		phase += 0.10f * values_offset;
		refresh_time += 1.0f / 60.0f;
	}

	{
		float average = 0.0f;
		for (int n = 0; n < IM_ARRAYSIZE(values); n++) {
			average += values[n];
		}
		average /= (float)IM_ARRAYSIZE(values);
		char overlay[32];
		sprintf(overlay, "avg %f", average);
		ImGui::PlotLines("Lines", values, IM_ARRAYSIZE(values), values_offset, overlay, -1.0f, 1.0f, ImVec2(0, 80.0f));
	}
}

// много умных действий c implot
void renderPlot_with_multi_axis() {
	static float xs[1001], xs2[1001], ys1[1001], ys2[1001], ys3[1001];
	for (int i = 0; i < 1001; ++i) {
		xs[i] = (i * 0.1f);
		xs2[i] = xs[i] + 10.0f;
		ys1[i] = sinf(xs[i]) * 3 + 1;
		ys2[i] = cosf(xs[i]) * 0.2f + 0.5f;
		ys3[i] = sinf(xs[i] + 0.5f) * 100 + 200;
	}

	static bool x2_axis = true;
	static bool y2_axis = true;
	static bool y3_axis = true;

	ImGui::Checkbox("X-Axis 2", &x2_axis);
	ImGui::SameLine();
	ImGui::Checkbox("Y-Axis 2", &y2_axis);
	ImGui::SameLine();
	ImGui::Checkbox("Y-Axis 3", &y3_axis);

	ImGui::BulletText("You can drag axes to the opposite side of the plot.");
	ImGui::BulletText("Hover over legend items to see which axis they are plotted on.");

	if (ImPlot::BeginPlot("Multi-Axis Plot", ImVec2(-1, 0))) {
		ImPlot::SetupAxes("X Axis 1", "Y Axis 1");
		ImPlot::SetupAxesLimits(0, 100, 0, 10);
		if (x2_axis) {
			ImPlot::SetupAxis(ImAxis_X2, "X-Axis 2", ImPlotAxisFlags_AuxDefault);
			ImPlot::SetupAxisLimits(ImAxis_X2, 0, 100);
		}
		if (y2_axis) {
			ImPlot::SetupAxis(ImAxis_Y2, "Y-Axis 2", ImPlotAxisFlags_AuxDefault);
			ImPlot::SetupAxisLimits(ImAxis_Y2, 0, 1);
		}
		if (y3_axis) {
			ImPlot::SetupAxis(ImAxis_Y3, "Y-Axis 3", ImPlotAxisFlags_AuxDefault);
			ImPlot::SetupAxisLimits(ImAxis_Y3, 0, 300);
		}

		ImPlot::PlotLine("f(x) = x", xs, xs, 1001);
		if (x2_axis) {
			ImPlot::SetAxes(ImAxis_X2, ImAxis_Y1);
			ImPlot::PlotLine("f(x) = sin(x)*3+1", xs2, ys1, 1001);
		}
		if (y2_axis) {
			ImPlot::SetAxes(ImAxis_X1, ImAxis_Y2);
			ImPlot::PlotLine("f(x) = cos(x)*.2+.5", xs, ys2, 1001);
		}
		if (y3_axis) {
			ImPlot::SetAxes(ImAxis_X2, ImAxis_Y3);
			ImPlot::PlotLine("f(x) = sin(x+.5)*100+200 ", xs2, ys3, 1001);
		}
		ImPlot::EndPlot();
	}
}

