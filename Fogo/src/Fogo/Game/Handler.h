#pragma once

#include "./UUID.h"
#include "./ContainerBase.h"
#include "./ContainerIndexKeyPair.h"
#include "./ComponentTree.h"
#include "./Hash.h"

namespace Fogo {

	template <typename Element>
	class Handler {
	private:
		template <typename Key, typename Elm = Element>
		using IndexedStore = ContainerBase<ContainerIndexKeyPair<Key>, Elm*, Hash<Key>>;
		
		Element * element;

	public:
		Handler(Element * element) : element(element) {

		}

		Handler() : Handler(nullptr) {
			// なんでいるんこれ
		}

		static Handler Create(Element * element) {
			return std::move(Handler(element));
		}

		// 継承元クラスでの取得
		template <typename ElementAs>
		Handler<ElementAs> as() const {
			static_assert(std::is_base_of<ElementAs, Element>());
			if (const auto & elementAs = Store::Get<ElementAs>(element->uuid)) {
				return *elementAs;
			}
			// 格納されていない場合新たに格納して返却
			return Store::BindAs<Element, ElementAs, false>(element, *ComponentTree::shared->getParent(element->uuid));
		}

		// インデックスの作成
		template <typename Elm = Element, typename Key>
		Handler & makeIndex(Key key) {
			IndexedStore<Key, Elm>::shared.insert(std::make_pair(ContainerIndexKeyPair<Key> { key, *ComponentTree::shared->getParent(element->uuid) }, element));
			return *this;
		}

		// インデックスの削除
		template <typename Elm = Element, typename Key>
		Handler & destroyIndex(Key key) {
			IndexedStore<Key, Elm>::shared.erase(ContainerIndexKeyPair<Key> { key, *ComponentTree::shared->getParent(element->uuid) });
			return *this;
		}

		// 親の挿げ替え
		Handler & assign(const UUID & newParentId) {
			ComponentTree::shared->create(newParentId, element->uuid);
			return *this;
		}

		// elementへのメンバアクセス
		Element * operator->() const {
			return element;
		}

		// elementへの参照を得る
		Element & operator*() const {
			return *element;
		}

		// elementのポインタを得る
		Element * operator&() const {
			return element;
		}

		// 親UUIDを得る
		UUID getParentUUID() const {
			return *ComponentTree::shared->getParent(element->uuid);
		}

		// 解放する
		void release() {
			delete element;
			element = nullptr;
		}
	};

	// elementがnullptrかチェックする
	template <typename Element>
	struct HandlerChecker {
		bool operator()(const Handler<Element> & handler) const {
			return &handler != nullptr;
		}
	};

}
