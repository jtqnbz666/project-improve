//读写锁
package main

import(
	"fmt"
	"sync"
	"time"
)

type SafeDict struct {
	data map[string]int 
	*sync.RWMutex 	//和sync.Mutex API 也有点不一样
}

func NewSafeDict(data map[string]int) *SafeDict {
	return &SafeDict{data, &sync.RWMutex{}}
}

func (d *SafeDict) Len() int {
	d.RLock()
	defer d.RUnlock()
	return len(d.data)
}

func (d *SafeDict) Put(key string, value int) (int, bool) {
	d.Lock()
	defer d.Unlock()
	old_value, ok := d.data[key]
	d.data[key] = value 
	return old_value, ok 
}

func (d *SafeDict) Get(key string) (int, bool) {
	d.RLock()
	defer d.RUnlock()
	old_value, ok := d.data[key]
	return old_value, ok
}

func (d *SafeDict) Delete(key string) (int, bool) {
	d.Lock()
	defer d.Unlock()
	old_value, ok := d.data[key] 
	if ok {
		delete(d.data, key)
	}
	return old_value, ok 
}

func write(d *SafeDict) {
	d.Put("banana", 6)
}

func read(d *SafeDict) {
	fmt.Println(d.Get("banana"))
}

func main() {
	d := NewSafeDict(map[string]int {
		"apple" : 2,
		"pear" : 3,
	})
	go read(d)
	write(d)
	time.Sleep(1 * time.Second)
}