<template>
    <div id="app" class="container">
        <div id="error" v-if="unhandledRejection">
            Ошибка <br>
            {{unhandledRejection.reason.message}}<br>
            <!--
                        <pre> стек в продакшене ужасный!!!
                            {{unhandledRejection.reason.stack}}
                        </pre>
            -->
        </div>

        <form @submit.prevent="1">
            <mu-tabs :value="operation" @change="operation_change">
                <mu-tab value="pay" title="Оплата"/>
                <mu-tab value="cancel" title="Отмена последней операции"/>
                <mu-tab value="fullList" title="Полный журнал"/>
                <mu-tab value="shortList" title="Краткий журнал"/>
                <mu-tab value="compare" title="Сверка итогов"/>
            </mu-tabs>
            <div v-show="operation === 'pay'">
                <mu-auto-complete id="search" labelFloat label="Номер договора или ФИО (Esc)" fullWidth
                                  :dataSource="autocomplete_dataSource" filter="noFilter"
                                  @select="autocomplete_select"/>

                <subscriber-view v-if="model.subscriber" v-bind:model="model.subscriber"
                                 @accountNumberChange="subscriberView_accountNumberChange"></subscriber-view>

                <template v-if="model.subscriber">
                    <div class="d-flex mt-5  align-items-stretch">
                        <mu-text-field class="" type="number" label="Сумма платежа" labelFloat fullWidth :min="0"
                                       v-model.number="model.sum"/>
                    </div>
                    <mu-raised-button primary class="" type="submit"
                                      label="Оплатить"
                                      :disabled="!(model.account && model.sum && model.sberbankServerStatus)"
                                      value="Оплатить (Enter)"
                                      @click="model.pay.bind(model)()"/>
                    <div id="sberbankServerStatusMessage" v-if="!model.sberbankServerStatus">Платежный сервис ПАО
                        "Сбербанк" недоступен
                    </div>
                </template>
            </div>
            <div v-if="operation === 'cancel'" class="d-flex mt-5">
                <mu-raised-button primary class="mx-auto" label="Отмена последней операции" @click="cancel_click"/>
            </div>
            <div v-if="operation === 'fullList'" class="d-flex mt-5">
                <mu-raised-button primary class="mx-auto" label="Показать полный журнал"
                                  @click="fullList_click"></mu-raised-button>
            </div>
            <div v-if="operation === 'shortList'" class="d-flex mt-5">
                <mu-raised-button primary class="mx-auto" label="Показать краткий журнал"
                                  @click="shortList_click"></mu-raised-button>
            </div>
            <div v-if="operation === 'compare'" class="d-flex mt-5">
                <mu-raised-button primary class="mx-auto" label="Сверка итогов"
                                  @click="compare_click"></mu-raised-button>
            </div>
            <div class="d-flex mt-5">
                <mu-paper v-if="model.cheq" class="cheq" :zDepth="2">{{model.cheq}}</mu-paper>
            </div>
            <pre>
                operation {{operation}}
                <!--{{ model.account}}-->
                <!--{{model.subscriber}}-->
                    </pre>
        </form>
    </div>
</template>

<script>
    import $ from 'jquery'
    import autocomplete from 'jquery-ui/ui/widgets/autocomplete'
    import debounce from 'lodash/debounce'
    import Rx from 'rxjs/Rx'
    //    let Rx = require('@reactivex/rxjs')

    import SubscriberView from './SubscriberView'

    let config = require('../../config/main')[process.env.NODE_ENV]
    console.log("NODE_ENV", process.env.NODE_ENV, "config", config)


    export default {

        name: 'app-view',
        data: function () {
            return {
                activeTab: 'tab1',
                operation: "pay",
                unhandledRejection: null,
                autocomplete_dataSource: [{text: "javascript", value: 1}]
            }
        },
        props: ["model"],
        components: {
            SubscriberView
        },
        computed: {},
        methods: {
            esc_click(){
                alert(123)
            },
            async fullList_click(){
                this.model.fullList()
            },

            async shortList_click(){
                this.model.shortList()
            },

            async compare_click(){
                this.model.compare()
            },

            async cancel_click(){
                if (confirm("Отменить последнюю операцию?")) {
                    this.model.cancel()
                }
            },
            handleTabChange(x){
                this.activeTab = x
                alert("change" + x)
            },
            handleActive(y){
                alert("active")
            },
            async operation_change(cur){
                this.operation = cur
            },
            /**
             * Ищет клиентов удовлетворяющих поиску
             * Преобразовывает массив объектов получаемый по http в правильный массив у которого
             * text- это отображаемое имя клиента, а value- идентификатор клиента
             *
             * @return <[{text, value}]> Promise
             */
            autocomplete_getDataSource(term){
                let url = new URL(`${config.server.schema}://${config.server.host}:${config.server.port}/${config.server.subscribersPath}`)
                url.searchParams.append("term", term)
//                console.log(url.href)
                return fetch(url)
                    .then(response => response.json())
                    .then(result => {
                        let fixedResult = result.map((eachResult) => {
                            return {
                                text: eachResult.label,
                                value: +eachResult.value,
                            }
                        })
                        console.log("search result", fixedResult)
                        return fixedResult
                    })
                    .catch((err) => {
                        throw new Error(`Autocomplete search Error ${url.href}: ${err}`)
                    })
            },

            /**
             * выбирает текущим клиентом того, кого оператор выбрал в автокомплите
             * и ресет формы reset()
             * @param event
             * @param ui
             */
            autocomplete_select(item, index) {
                this.model.getSubscriber(item.value)
                    .then(subscriber => {
                        this.model.cheq = null
                        this.model.subscriber = subscriber;
                    })
                    .catch(err => {
                        throw new Error(`Autocomplete select Error: ${err}`)
                    });
                document.forms[0].reset()
            },

            subscriberView_accountNumberChange(value){
                console.log("account changed", value);
                this.model.account = value;
            },
        },
        beforeCreate(){
            window.addEventListener('unhandledrejection', (event) => {
                this.$data.unhandledRejection = event;
            });
        },
        mounted() {
            window.addEventListener("keyup", function(e){
                if (e.keyCode==27){
                  $("#search input").focus()
                }
            })
            Rx.Observable.fromEvent(document.querySelector("#search input"), "input")
                .do(() => {
                    this.$data.autocomplete_dataSource = []
                })
                .debounceTime(300)
                .distinctUntilChanged()
                .switchMap(event => event.target.value ? Rx.Observable.fromPromise(this.autocomplete_getDataSource(event.target.value)) : Rx.Observable.of([]))
                .subscribe(datasource => {
                    this.$data.autocomplete_dataSource = datasource
                })
        }
    }
</script>

<style>
    /*    * {
            font-family: 'Avenir', Helvetica, Arial, sans-serif;
            -webkit-font-smoothing: antialiased;
            -moz-osx-font-smoothing: grayscale;
            font-size: 20px;
        }*/

    /*    #search {
            width: 20em;

            display: block;
            font-size: 1.5em;
        }*/

    /*    #search2 input {
            font-size: 1.5em;
        }*/

    /*
        #sum {
            width: 10em;

            font-size: 1.5em;
            margin-top: 2em;
        }

        #pay {
            font-size: 1.5em;
        }
    */

    #error {
        background-color: darkred;
        color: white;
        margin: 1em 0;
    }

    #sberbankServerStatusMessage {
        background-color: darkred;
        color: white;
        margin: 1em 0;
    }

    .cheq {
        white-space: pre;
        margin: 3em;
        padding: 1em;
    }
</style>
