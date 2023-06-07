#pragma once

#include "Test.h"

namespace test
{

	/* �����ɫ: ָ������ָ����ɫ��䱳��ɫ ˢ�� */
	class TestClearColor : public Test
	{
	private:
		const char* m_Label;
		float m_ClearColor[4];
	public:
		TestClearColor();
		~TestClearColor();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
	};
}