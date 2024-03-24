#pragma once

/* Documentation:
	* --------------------------------------------------------------------
	* USAGE: returnType returnObject = (__WeakPointerObject__->*(__Address__to__function__))(__arguments__)
	*	- operator->* is overloaded to take in function and arguments
	*	- [NOTE (Optional): if possible use "std::move_if_noexcept"]
	*	- You can also use Execute [but it doesn't have a return type at the moment]
	*
	* --------------------------------------------------------------------
	* EXAMPLE Below:
	*	class ExampleClass
	*	{
	*		public:
	*			ExampleClass() = default;
	*
	*			double TestFunction(double a)
	*			{
	*				std::cout << "Test Function being executed!\n";
	*				return a;
	*			}
	*	}
	*
	*	int main()
	*	{
	*		auto sharedExample = std::make_shared<ExampleClass>();
	*	
	*		const NightOwl::Memory::WeakPointer<ExampleClass> weakExample { std::move_if_noexcept(sharedExample) };
	*
	*		auto result = (weakExample->*(&ExampleClass::TestFunction))(100);
	*		std::cout << "Result: " << result << "\n";
	*	}
	* --------------------------------------------------------------------
*/

namespace AnimationEngine::Memory
{
	template <typename T>
	class WeakPointer
	{
	public:
		WeakPointer();

		WeakPointer(const std::weak_ptr<T>& pointer);

		WeakPointer(std::weak_ptr<T>&& pointer) noexcept;

		WeakPointer& operator=(const std::weak_ptr<T>& pointer);

		WeakPointer& operator=(std::weak_ptr<T>&& pointer) noexcept;

		void CaptureWeakPointer(std::weak_ptr<T> pointer) noexcept;

		template <typename Func>
        decltype(auto) operator->*(Func&& func) const;

		// Execute without return value, for return value use operator->* overload
		template <typename Func, typename... Args>
		void Execute(Func&& func, Args&&... args) const;

		std::shared_ptr<T> operator->() const;

		std::shared_ptr<T> GetShared() const;

		std::weak_ptr<T> GetWeakPointer() const;

	private:
		std::weak_ptr<T> weakPointer;

		bool validation;

		std::shared_ptr<T> ValidatePointer() const;
	};
}

#include "WeakPointer.inl"
