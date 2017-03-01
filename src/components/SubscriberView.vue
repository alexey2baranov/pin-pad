<template>
    <mu-card>
        <mu-card-title :title="model.fullName" :subTitle="'Договор № '+model.contrNumber"></mu-card-title>
        <mu-card-text class="accounts">
            <div v-for="eachAccount in model.accounts" class="mb-5">
                <h3>
                    <input :id="eachAccount.id" type="radio" name="account" :value="eachAccount.number" @change="accountNumberChange(eachAccount)">
                    <label :for="eachAccount.id" title="Текущий остаток на счете - абонентская плата = остаток на счете после списания абонентской платы">{{eachAccount.number}} ({{floor(eachAccount.saldo)}} - {{getTotalCost(eachAccount)}} = {{getMinimumPayment(eachAccount)}} руб) </label>
                </h3>
                <div class="services">
                    <div v-for="eachService in eachAccount.services" class="service-block">
                        <span class="service-status">{{serviceStatus[+eachService.status]}}</span>
                        <span class="service-type">{{serviceType[+eachService.type]}}</span>
                        <span class="tariffPlanName">{{eachService.tariffPlanName}}</span>
                        <span class="cost">{{eachService.cost}} руб.</span>
                    </div>
                </div>
            </div>
        </mu-card-text>
    </mu-card>
</template>

<script>
    require('jquery-ui/themes/base/theme.css');
    require('jquery-ui/themes/base/all.css');
    require('jquery-ui/themes/base/base.css');
    require('jquery-ui/themes/base/autocomplete.css');

    export default {
        name: 'subscriber-view',
        data () {
            return {
                serviceType: {
                    1: "Телефон",
                    101: "Телефон",
                    3: "DialUp карта",
                    11: "Учет траффика для выделенных линий",  //интернет для юриков с выделенкой и стат IP
                    15: "IP-TV",
                },
                serviceStatus: {
                    0: "Создана",
                    1: "Активна",
                    2: "Заблокирована",
                    3: "Закрыта",
                    4: "Просрочена"
                }
            }
        },
        props: ['model'],
        methods:{
            accountNumberChange(account){
                this.$emit("accountNumberChange", account);
            },
            getMinimumPayment(account){
                let result= Math.floor(account.saldo- this.getTotalCost(account));
                return result;
            },
            getTotalCost(account){
                let result= 0;
                for(let eachService of account.services){
                    result+= eachService.cost;
                }
                return Math.ceil(result);
            },
            floor(arg){
                return Math.floor(arg)
            }
        }
    }
</script>

<!-- Add "scoped" attribute to limit CSS to this component only -->
<style scoped>
</style>
