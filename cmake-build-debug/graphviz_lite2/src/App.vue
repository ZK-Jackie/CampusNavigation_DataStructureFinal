<template>
  <div id="app">
    <div class="graph" v-html="svg"></div>
  </div>
</template>

<script>
import { instance } from "@viz-js/viz";
  import { nav } from "./navigation";
  
  export default {
    data(){
      return{
        svg: "",
      }
    },
    //nav变化触发watch，更新svg
    watch:{
      nav:{
        navChange(newVal){
          this.svg = newVal;
        },
        immediate: true,
      }
    },
    mounted() {
      var outside = this;
      instance().then((viz) => {
        //绑定解析对象
        const svg = viz.renderSVGElement(nav);
        //第一次改变不会触发watch，则此处初始化
        outside.svg = svg.outerHTML;
      });
    },
  };
</script>


<style>
#app {
  font-family: Avenir, Helvetica, Arial, sans-serif;
  -webkit-font-smoothing: antialiased;
  -moz-osx-font-smoothing: grayscale;
  text-align: center;
  color: #2c3e50;
}

nav {
  padding: 30px;
}

nav a {
  font-weight: bold;
  color: #2c3e50;
}

nav a.router-link-exact-active {
  color: #42b983;
}
</style>
