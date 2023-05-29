import { Body, Controller, Get, Post, Res } from '@nestjs/common';
import { AppService } from './app.service';
import { join } from 'path';

@Controller()
export class AppController {
  constructor(private readonly appService: AppService) {}

  @Get()
  renderHome(@Res() res) {
    res.sendFile(join(__dirname,"../src/","public/html", "index.html"))
  }
  @Post('add_reading')
  async create(@Body('path') pathData: any[]) {
    const createdPath = await this.appService.create(pathData);
    
    return { code:200, message: 'Paths created successfully'};
  }

  @Get("get_all_readings")
  async findAll() {
    const paths = await this.appService.findAll();
    return { paths };
  }
}
