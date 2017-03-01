import Vue from 'vue'
import AppView from 'src/components/AppView'
import App from 'src/App'

describe('AppView', () => {
    it.skip('should render correct contents', () => {
        const vm = new Vue({
            el: document.createElement('div'),
            props: {
                model: new App()
            },
            render: (h) => h(AppView)
        })
        expect(vm.$el.querySelector('form')).exist
    })

    it('should autocomplete clients', function (done) {
        const vm = new Vue(AppView)

        vm.autocomplete_getDataSource('789')
            .then(data => {
                expect(data).a("Array")
                expect(data.length).equals(10)
                expect(data[0]).property("text")
                expect(data[0]).property("value")

                done()
            })
            .catch(err=> {
                done(err)
            })
    })
})
